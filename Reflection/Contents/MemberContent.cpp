//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberContent.h"
#include "Reflection/Attributes/ShortSyntaxAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "Reflection/Utilities/DefaultArgumentExtracter.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Attributes/MethodToStringAttribute.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Types/DynamicTypeTraits.h"
#include "Reflection/Attributes/ExpressionExpanderAttribute.h"
#include "TypeContent.h"
#include "ObjectContent.h"
#include "ContentObjectAtIndex.h"
#include "ObjectContent.h"
#include "TextContent.h"
#include "ShortSyntax/ContentShortSyntaxExpander.h"
#include "TypedTextContent.h"
#include "ContentExpressionEvaluator.h"
#include "Impl/ContentUtility.h"
#include "Services/ServiceProvider.h"
#include "../Attributes/ContainerAttribute.h"
#include "../Attributes/AttributeCollectionService.h"
#include "../Attributes/TupleAttribute.h"
#include "Services/Allocators/PersistenceControl.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    using namespace Attributes;

    MemberContent::MemberContent(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, const std::string& text)
        : m_member(member)
        , m_arguments(arguments)
        , m_text(text)
    {
        if (!m_member)
            throw std::runtime_error(__FUNCTION__ ": member cannot be nullptr");
    }

    std::shared_ptr<IContent> MemberContent::Parse(const Members::MemberPointer& member, const std::string& text, const std::shared_ptr<IContent>& existingContent)
    {
        auto scriptingService = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>();
        if (scriptingService)
        {
            try {
                std::vector<std::string> args;
                std::string memberName;
                if (scriptingService->TrySplitFunctionArguments(text + ";", memberName, args))
                {
                    if (_strcmpi(memberName.c_str(), member->GetActualName().c_str()) == 0 && args.size() >= member->GetMinimumArgumentCount() && args.size() <= member->GetMaximumArgumentCount())
                    {
                        std::vector<std::shared_ptr<IContent>> arguments;
                        for (size_t i = 0; i < args.size(); ++i)
                            arguments.push_back(std::make_shared<TypedTextContent>(args[i], member->GetArgumentInfo(i)->GetTypeInfo()));
                        return std::make_shared<MemberContent>(member, arguments, text);
                    }
                }
                ContentExpressionEvaluator expressionEvaluator(scriptingService);
                Objects::Object object = expressionEvaluator.EvaluateExpression(text);
                if (!object.IsConvertibleTo(member->GetReturnType()))
                {
                    ContentExpressionResult result = ExpandShortSyntaxExpression(text, member, expressionEvaluator, existingContent, false);
                    return result.GetContent();
                }
            }
            catch (...)
            {
            }
        }
        return nullptr;
    }

    bool MemberContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        if (!IsValid())
            return false;
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
		if (ContentUtility::IsConvertibleTo(typeLibrary, m_member->GetReturnType(), typeInfo))
			return true;
		if(CanConvertToObject(typeLibrary))
			return ContentUtility::IsConvertibleTo(GetObject(), typeInfo);
		return false;
    }

    Types::DecoratedTypeInfo MemberContent::GetTypeInfo() const
    {
        return m_member->GetReturnType();
    }

    Objects::Object MemberContent::GetObject() const
    {
        std::vector<Variants::Variant> args;
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                return Objects::Object();
            args.push_back(arg->GetObject().GetVariant());
        }
        try {
            Services::Allocators::PersistenceControl control(false);
            auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
            return Objects::Object(typeLibrary, m_member->Invoke(args));
        }
        catch (...)
        {
            return Objects::Object();
        }
    }

    bool MemberContent::HasStoredObject() const
    {
        if (m_member && m_member->IsConst())
        {
            for (const auto& arg : m_arguments)
            {
                if (!arg)
                    return false;
                if (!arg->HasStoredObject())
                {
                    if (!arg->HasScriptText())
                        return false;
                    auto scriptingService = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>();
                    if (!scriptingService)
                        return false;
                    if (!scriptingService->IsLookupExpression(arg->GetScriptText()+";"))
                        return false;
                }
            }
            return true;
        }
        return false;
    }

    std::string MemberContent::GetDisplayText() const
    {
        if (!m_text.empty())
            return m_text;
        if (!m_member->GetAttributeCollection().HasAttribute<ShortSyntaxAttribute>())
            return GetScriptText();
        const auto& shortSyntax = m_member->GetAttributeCollection().GetAttribute<ShortSyntaxAttribute>();
        std::string displayText;
        std::string optionalText;
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto argument = TryGetArgument(m_member, i);
            if (!argument || i >= shortSyntax.GetArguments().size())
                return GetScriptText();
            if (shortSyntax.GetArguments()[i] == ShortSyntaxArgument::ShortSyntax)
            {
                if (!displayText.empty())
                    displayText += " ";
                displayText += argument->GetDisplayText();
            }
            if (shortSyntax.GetArguments()[i] == ShortSyntaxArgument::ScriptSyntax)
            {
                if(i>0 && IsDefaultArgument(i))
                    optionalText += argument->GetScriptText();
                else {
                    if (!displayText.empty())
                        displayText += " ";
                    if (!optionalText.empty())
                        displayText += optionalText + " ";
                    displayText += argument->GetScriptText();
                    optionalText.clear();
                }
            }
        }
        return displayText;
    }

	bool MemberContent::CanConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary) const
	{
		if (!m_member)
			return false;
		if (m_member->IsConst())
			return true;
		if(m_member->GetMemberType() != Members::MemberType::TypeConstructor)
			return false;
		if (Attributes::GetPointerToAttributeOrNull<Attributes::ContainerAttribute>(typeLibrary, m_member->GetReturnType()))
			return true;
		if (Attributes::GetPointerToAttributeOrNull<Attributes::TupleAttribute>(typeLibrary, m_member->GetReturnType()))
			return true;
		return false;
	}

	size_t MemberContent::GetActualArity() const
    {
        if (m_member->GetMinimumArgumentCount() == 0 && m_member->GetMemberType() == Members::MemberType::TypePropertyGet)
            return 0;
        //Support for variadic arguments.
        else if (m_arguments.size() > m_member->GetMinimumArgumentCount() && m_arguments.size() < m_member->GetMaximumArgumentCount())
            return m_arguments.size();
		size_t arity = m_member->GetMinimumArgumentCount();
		while (arity < m_member->GetArity())
		{
			if (!m_member->GetArgumentInfo(arity)->HasDefault())
				return arity;
			++arity;
		}
		return arity;
    }

    std::string MemberContent::GetScriptTextUsingAttribute() const
    {
        std::vector<std::pair<std::string, Objects::Object>> args;
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                throw std::exception();
            args.push_back(std::make_pair(arg->GetScriptText(),arg->GetObject()));
        }
        return m_member->GetAttributeCollection().GetAttribute<Attributes::MethodToStringAttribute>().ToString(args);
    }

    std::string MemberContent::GetScriptText() const
    {
        if (m_arguments.size() > m_member->GetMaximumArgumentCount())
            return m_member->GetActualName() + " requires a maximum of " + std::to_string(m_member->GetMaximumArgumentCount()) + " arguments. " + std::to_string(m_arguments.size()) + " given.";
        if(m_member->GetAttributeCollection().HasAttribute<Attributes::MethodToStringAttribute>())
        {
            try {
                return GetScriptTextUsingAttribute();
            }
            catch(...)
            {}
        }
        std::vector<std::string> arguments;
        std::vector<std::string> optionalArguments;
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
            {
                if (m_arguments.size() < m_member->GetMinimumArgumentCount() && i >= m_arguments.size())
                    arguments.push_back(m_member->GetActualName() + " requires a minimum of " + std::to_string(m_member->GetMinimumArgumentCount()) + " arguments. Only " + std::to_string(m_arguments.size()) + " given.");
                else
                    arguments.push_back("Argument " + std::to_string(i) + " is null");
            }
            else /*if (m_member->GetArgumentInfo(i)->HasDefault())
            {
                auto typeLibrary = Services::ServiceProvider::Instance().GetService<Reflection::TypeLibraries::ITypeLibrary>();
                ObjectContent defaultValue(Reflection::Objects::Object(typeLibrary, m_member->GetArgumentInfo(i)->GetDefault()));

                if (defaultValue.IsContentEqual(*arg))
                    optionalArguments.push_back(arg->GetScriptText());
                else
                {
                    arguments.insert(arguments.end(), optionalArguments.begin(), optionalArguments.end());
                    optionalArguments.clear();
                }
            }
            if(optionalArguments.empty())*/
                arguments.push_back(arg->GetScriptText());
        }
        return m_member->ToString(arguments);
    }

    bool MemberContent::IsContentEqual(const IContent& other) const
    {
        if (GetActualArity() == 0)
        {
            if (HasStoredObject() || other.HasStoredObject())
                return GetObject() == other.GetObject();
            else
                return other.TryGetBestMember() == m_member;
        }
            
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto lhs = TryGetArgument(m_member, i);
            auto rhs = other.TryGetArgument(m_member, i);
			if (!lhs || !rhs)
			{
				return GetObject() == other.GetObject();
			}
            if (!lhs->IsContentEqual(*rhs))
                return false;
        }
        return true;
    }

    bool MemberContent::IsValid() const
    {
        if (m_arguments.size() > m_member->GetMaximumArgumentCount())
            return false;
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                return false;
            if (!arg->IsConvertibleTo(m_member->GetArgumentInfo(i)->GetTypeInfo()))
                return false;
        }
        return true;
    }

    std::shared_ptr<IContent> MemberContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        if (member == m_member)
        {
            if (index < m_arguments.size())
                return m_arguments[index];
            
            auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
            Utilities::ArgumentFromObjectExtracter extracter(typeLibrary, member, index);
            Objects::Object result(typeLibrary, Utilities::DefaultArgumentExtracter(typeLibrary, extracter.GetGetter(), member, index).TryGetDefaultValue());
            if (!result.IsValid())
                return nullptr;
            if (result.IsConvertibleTo<TypeLibraries::TypePointer>())
            {
                TypeLibraries::TypePointer type = result.As<TypeLibraries::TypePointer>();
                return std::make_shared<TypeContent>(Types::DecoratedTypeInfo(type->GetType(), Types::TypeDecoration::Reference | Types::TypeDecoration::Const));
            }
            else
                return std::make_shared<ObjectContent>(result);
        }
        else
            return ContentUtility::TryGetArgument(GetObject(), member, index);
    }

    Members::MemberPointer MemberContent::TryGetBestMember() const
    {
        if (HasStoredObject())
            return ObjectContent(GetObject()).TryGetBestMember();
        return m_member;
    }

    Members::MemberPointer MemberContent::GetStoredMember() const
    {
        return m_member;
    }

    std::vector<std::string> MemberContent::DoErrorCheck() const
    {        
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        auto typeFormatter = std::make_shared<TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        if (m_arguments.size() > m_member->GetMaximumArgumentCount())
            return { m_member->GetActualName() + " does not take " + std::to_string(m_arguments.size()) };
        for (size_t i = 0; i < GetActualArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                return { "The " + std::to_string(i) + " argument of " + m_member->GetActualName() + " is not given" };
            if (!arg->IsConvertibleTo(m_member->GetArgumentInfo(i)->GetTypeInfo()))
                return { "The " + std::to_string(i) + " argument of " + m_member->GetActualName() + ": '" + arg->GetDisplayText() + "' + can not be converted to " + typeFormatter->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) };
        }
        return {};
    }

    std::shared_ptr<IContent> MemberContent::InvokeProperty(const std::string& propertyName) const
    {
        size_t index = Utilities::DefaultArgumentExtracter::GetIndexOfNamedConstructorArgument(m_member, propertyName);
        if (index < GetActualArity())
            return TryGetArgument(m_member, index);
        else
            return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> MemberContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return InvokeProperty(propertyGetter->GetName());
    }

    std::shared_ptr<IContent> MemberContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        if(Types::IsBaseOf(m_member->GetConversionGraph(), typeInfo, m_member->GetReturnType()))
            return std::make_shared<MemberContent>(m_member, m_arguments);
        else
            return IContent::TryChangeType(typeInfo, existingContent);
    }

    bool MemberContent::HasScriptText() const
    {
        return true;
    }
    
    std::shared_ptr<IContent> MemberContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    Objects::Object MemberContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(GetObject(), typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> MemberContent::Split() const
    {
        if (IsCollection())
        {
            std::vector<std::shared_ptr<IContent>> results;
            std::vector<std::shared_ptr<IContent>> args(m_arguments.size());
            SplitRecursive(results, args, 0);
            return results;
        }
        return { std::make_shared<MemberContent>(*this) };
    }

    void MemberContent::SplitRecursive(std::vector<std::shared_ptr<IContent>>& results, std::vector<std::shared_ptr<IContent>>& args, int index) const
    {
        if (index < args.size())
        {
            if (!m_arguments[index])
                return;
            for (const auto& arg : m_arguments[index]->Split())
            {
                args[index] = arg;
                SplitRecursive(results, args, index + 1);
            }
        }
        else
            results.push_back(std::make_shared<MemberContent>(m_member, args));
    }

    bool MemberContent::IsCollection() const
    {
        for (const auto& content : m_arguments)
        {
            if (content && content->IsCollection())
                return true;
        }
        return false;
    }

    bool MemberContent::IsDefaultArgument(size_t index) const
    {
        if (index < m_member->GetMinimumArgumentCount())
            return false;
        if (index >= m_arguments.size())
            return true;
        if (!m_arguments[index])
            return false;
        try {
            auto arg = m_arguments[index]->GetObject().ConvertTo(m_member->GetArgumentInfo(index)->GetTypeInfo());
            auto defaultArg = Objects::Object(arg.GetTypeLibrary(), m_member->GetArgumentInfo(index)->GetDefault());
            return arg == defaultArg.ConvertTo(m_member->GetArgumentInfo(index)->GetTypeInfo());
        }
        catch (...)
        {
            return false;
        }
    }

}}}}

