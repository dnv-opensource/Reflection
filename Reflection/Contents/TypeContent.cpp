//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeContent.h"
#include "Services/ServiceProvider.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Utilities/ConstructorsFromTypeExtracter.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/Utilities/DefaultArgumentExtracter.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Utilities/ArgumentFromObjectExtracter.h"
#include "MemberContent.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Impl/ContentUtility.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    TypeContent::TypeContent(const Types::DecoratedTypeInfo& typeInfo)
        : m_type(typeInfo)
    {

    }

    bool TypeContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->IsConvertibleTo(typeInfo);
        return false;
    }

    Types::DecoratedTypeInfo TypeContent::GetTypeInfo() const
    {
        return m_type;
    }

    Objects::Object TypeContent::GetObject() const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->GetObject();
        return Objects::Object();
    }

    std::string TypeContent::GetDisplayText() const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->GetDisplayText();
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        return typeLibrary->GetTypeFormatter()->FormatType(m_type);
    }

    std::string TypeContent::GetScriptText() const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->GetScriptText();
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        return typeLibrary->GetTypeFormatter()->FormatType(m_type);
    }

    bool TypeContent::IsContentEqual(const IContent& other) const
    {
        if (!other.IsValid())
            return false;
        if (GetTypeInfo() != other.GetTypeInfo())
            return false;
        if (dynamic_cast<const TypeContent*>(&other) != nullptr)
            return true;
        auto member = TryGetBestMember();
        if(member)
        {
            if (member->GetArity() == 0)
                return GetObject() == other.GetObject();
            for (size_t i = 0; i < member->GetArity(); ++i)
            {
                auto lhs = TryGetArgument(member, i);
                auto rhs = other.TryGetArgument(member, i);
                if (!lhs || !rhs)
                    return false;
                if (!lhs->IsContentEqual(*rhs))
                    return false;
            }
            return true;
        }
        return false;
    }

    bool TypeContent::HasStoredObject() const
    {
        return false;
    }

    bool TypeContent::IsValid() const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->IsValid();
        return false;
    }

    Members::MemberPointer TypeContent::TryGetBestMember() const
    {
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        auto best = Utilities::ConstructorsFromTypeService::GetService(typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(m_type, true);
        return Utilities::ConstructorsFromTypeExtracter(typeLibrary).GetBestMemberFromType(best, m_type);
    }

    std::shared_ptr<IContent> TypeContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return MemberContent(member).TryGetArgument(member, index);
    }

    std::vector<std::string> TypeContent::DoErrorCheck() const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->DoErrorCheck();
        else
        {
            auto typeLibrary = Services::ServiceProvider::Instance().TryGetService<TypeLibraries::ITypeLibrary>();
            auto typeFormatter = std::make_shared<TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
            return { "Unable to construct a default representation of " + typeFormatter->FormatType(m_type) };
        }
    }

    std::shared_ptr<IContent> TypeContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        if (typeInfo.GetTypeInfo() != m_type.GetTypeInfo() || existingContent)
            return IContent::TryChangeType(typeInfo, existingContent);
        return std::make_shared<TypeContent>(typeInfo);            
    }

    std::shared_ptr<IContent> TypeContent::InvokeProperty(const std::string& propertyName) const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->InvokeProperty(propertyName);
        else
            return nullptr;
    }

    std::shared_ptr<IContent> TypeContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        auto defaultContent = TryConstructDefaultMember();
        if (defaultContent)
            return defaultContent->InvokeProperty(propertyGetter);
        else
            return nullptr;
    }

    Objects::Object TypeContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(GetObject(), typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> TypeContent::Split() const
    {
        return { std::make_shared<TypeContent>(*this) };
    }

    bool TypeContent::IsCollection() const
    {
        return false;
    }

    bool TypeContent::HasScriptText() const
    {
        return true;
    }

    std::shared_ptr<IContent> TypeContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    std::shared_ptr<IContent> TypeContent::TryConstructDefaultMember() const
    {
        auto member = TryGetBestMember();
        if (member)
        {
            return std::make_shared<MemberContent>(member);
        }
        return nullptr;
    }

}}}}

