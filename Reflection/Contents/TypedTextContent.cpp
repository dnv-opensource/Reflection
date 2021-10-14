//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedTextContent.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Attributes/ValidationAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/UndefinedAttribute.h"
#include "ContentObjectAtIndex.h"
#include "Reflection/Members/IMember.h"
#include "ShortSyntax/ContentExpressionResult.h"
#include "ShortSyntax/ContentShortSyntaxExpander.h"
#include "TypedLookupContent.h"
#include "ContentExpressionParser.h"
#include "ContentExpressionEvaluator.h"
#include "Impl/ContentUtility.h"
#include "ObjectContent.h"
#include "../Attributes/AttributeCollectionService.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    TypedTextContent::TypedTextContent(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent)
        : m_displayText(ContentUtility::StripQuotes(text))
        , m_typeInfo(typeInfo)
    {
        ContentExpressionParser parser(typeInfo);
        Objects::Object existing;
        if (existingContent)
            existing = existingContent->GetObject();
        m_scriptText = ContentUtility::ConditionallyAddQuotes(parser.ParseExpression(ContentUtility::ConditionallyAddQuotes(m_displayText, typeInfo), existing), typeInfo);
        if (m_scriptText.empty())
            m_scriptText = ContentUtility::ConditionallyAddQuotes(m_displayText, typeInfo);
        m_parseError = parser.GetErrorMessage();
        m_object = parser.GetParsedObject();
        if (m_object.IsConvertibleTo(typeInfo))
            m_object = m_object.ConvertTo(typeInfo);
        else if(m_object.IsValid())
            m_object = m_object.ConvertToDynamicType();
    }

    TypedTextContent::TypedTextContent(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const Objects::Object& object, bool useTextAsScriptText)
        : m_displayText(ContentUtility::StripQuotes(text))
        , m_typeInfo(typeInfo)
        , m_object(object)
        , m_scriptText(ContentUtility::ConditionallyAddQuotes(text,typeInfo))
    {
        if (m_object.IsConvertibleTo(typeInfo))
            m_object = m_object.ConvertTo(typeInfo);
        if (!useTextAsScriptText)
            m_scriptText = ObjectContent(object).GetScriptText();
    }

    std::shared_ptr<IContent> TypedTextContent::Parse(const std::string& text, const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent /*= nullptr*/)
    {
        std::string localText = ContentUtility::ConditionallyAddQuotes(text, typeInfo);
        auto scriptingService = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>();
        if (scriptingService)
        {
            if (scriptingService->IsLookupExpression(localText +";"))
                return std::make_shared<TypedLookupContent>(localText, typeInfo);
            try {
                ContentExpressionEvaluator expressionEvaluator(scriptingService);
                Objects::Object object = expressionEvaluator.EvaluateExpression(localText);
                if (object.IsConvertibleTo(typeInfo))
                    return std::make_shared<TypedTextContent>(localText, typeInfo, object);
                else
                {
                    ContentExpressionResult result = ExpandExpression(localText, typeInfo, expressionEvaluator, existingContent, false);
                    if (result.GetContent())
                        return result.GetContent();
                }
            }
            catch (...)
            {
            }
        }
        return std::make_shared<TypedTextContent>(localText,typeInfo, existingContent);
    }

    bool TypedTextContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::IsConvertibleTo(m_object, typeInfo);
    }

    Types::DecoratedTypeInfo TypedTextContent::GetTypeInfo() const
    {
        if (IsValid() || !m_object.IsValid())
            return m_typeInfo;
        else
            return m_object.GetDecoratedTypeInfo();
    }

    Objects::Object TypedTextContent::GetObject() const
    {
        return m_object;
    }

    std::string TypedTextContent::GetDisplayText() const
    {
        return m_displayText;
    }

    std::string TypedTextContent::GetScriptText() const
    {
        return m_scriptText;
    }

    bool TypedTextContent::IsContentEqual(const IContent& other) const
    {
        if (!IsValid())
            return IsTextEqual(other);
        if (!other.HasStoredObject())
            return other.IsContentEqual(*this);
        else
        {
            auto obj2 = other.GetObject();
            if (Reflection::Attributes::IsUndefined(m_object) && Reflection::Attributes::IsUndefined(obj2))
                return true;
            return m_object == obj2;
        }
    }

    bool TypedTextContent::HasStoredObject() const
    {
        return true;
    }

    bool TypedTextContent::IsValid() const
    {
        return m_parseError.empty() && m_object.IsConvertibleTo(m_typeInfo);
    }

    std::vector<std::string> TypedTextContent::DoErrorCheck() const
    {
        if (m_scriptText.empty() && !m_object.IsValid())
        {
            return { "No value set" };
        }
        if (!m_object.IsValid() || !m_parseError.empty())
        {
            auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
            auto typeFormatter = std::make_shared<TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());

            std::vector<std::string> error = { "Input does not represent a " + typeFormatter->FormatType(m_typeInfo) };
            if (!m_parseError.empty())
                error.push_back(m_parseError);
            else
                error.push_back("Invalid script expression: '" + m_displayText + "'");
            return error;
        }
        else if(m_object.GetTypeLibrary())
        {
            auto type = m_object.GetTypeLibrary()->LookupType(m_typeInfo.GetTypeInfo());
            if (auto attribute = Attributes::GetPointerToAttributeOrNull<Attributes::ValidationAttribute>(type))
            {
                if (attribute->GetStringArguments().size() == 1 && attribute->GetStringArguments().at(0) == "")
                {
                    try {
                        attribute->InvokeFunction({ m_object });
                    }
                    catch (std::exception& e)
                    {
                        return {
                            type->GetName() + " is invalid.",
                            e.what()
                        };
                    }
                }
            }
        }
        return {};
    }

    std::shared_ptr<IContent> TypedTextContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        if (m_object.IsConvertibleTo(typeInfo))
        {
            return std::make_shared<TypedTextContent>(m_scriptText, typeInfo, m_object);
        }
        else
            return IContent::TryChangeType(typeInfo, existingContent);
    }

    std::shared_ptr<IContent> TypedTextContent::TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        if(!IsValid() && m_object.IsValid())
        {
            if (TryGetBestMember() == member)
                return std::make_shared<TypedTextContent>(m_scriptText, member->GetReturnType(), m_object);
        }
        return nullptr;
    }

    std::shared_ptr<IContent> TypedTextContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        auto arg = ContentUtility::TryGetArgument(GetObject(), member, index);
        if (!arg || !arg->IsValid() || !IsValid())
            return arg;
        auto scriptingService = Services::ServiceProvider::Instance().GetService<IReflectionContentsService>();
        std::vector<std::string> args;
        std::string memberName;
        if (scriptingService->TrySplitFunctionArguments(m_scriptText+";", memberName, args) && 
            memberName == member->GetActualName() && 
            args.size() >= member->GetMinimumArgumentCount() && 
            args.size() <= member->GetMaximumArgumentCount()
            )
            return std::make_shared<TypedTextContent>(args[index], member->GetArgumentInfo(index)->GetTypeInfo(), arg->GetObject());
        return arg;
    }

    bool TypedTextContent::HasScriptText() const
    {
        return true;
    }

    std::shared_ptr<IContent> TypedTextContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    std::shared_ptr<IContent> TypedTextContent::InvokeProperty(const std::string& propertyName) const
    {
        return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> TypedTextContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return ContentUtility::InvokeProperty(*this, propertyGetter);
    }

    Objects::Object TypedTextContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(m_object, typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> TypedTextContent::Split() const
    {
        return { std::make_shared<TypedTextContent>(*this) };
    }

    bool TypedTextContent::IsCollection() const
    {
        return false;
    }

}}}}

