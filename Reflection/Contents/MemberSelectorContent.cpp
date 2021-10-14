//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberSelectorContent.h"
#include "Reflection/Utilities/MemberItem.h"
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"
#include "Reflection/Types/DefaultTypeFormatter.h"
#include "Reflection/Utilities/DefaultArgumentExtracter.h"
#include "MemberContent.h"
#include "IReflectionContentsService.h"
#include "Impl/ContentUtility.h"
#include "Services/ServiceProvider.h"
#include "Services/Allocators/PersistenceControl.h"


namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    MemberSelectorContent::MemberSelectorContent(const Members::MemberPointer& member)
        : m_member(member)
    {

    }

    bool MemberSelectorContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        return ContentUtility::IsConvertibleTo(typeLibrary, m_member->GetReturnType(), typeInfo);
    }

    Types::DecoratedTypeInfo MemberSelectorContent::GetTypeInfo() const
    {
        return m_member->GetReturnType();
    }

    Objects::Object MemberSelectorContent::GetObject() const
    {
        std::vector<Variants::Variant> args;
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg || !arg->IsValid())
                return Objects::Object();
            args.push_back(arg->GetObject().ToLValue().GetVariant());
        }
        try {
            Services::Allocators::PersistenceControl control(false);
            return Objects::Object(Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>(), m_member->Invoke(args));
        }
        catch (...)
        {
            return Objects::Object();
        }
    }

    bool MemberSelectorContent::HasStoredObject() const
    {
        return false;
    }

    std::string MemberSelectorContent::GetDisplayText() const
    {
        if (auto service = Services::ServiceProvider::Instance().TryGetService<IReflectionContentsService>())
        {
            return ToString(Utilities::MemberItem(m_member, nullptr), service->GetDisplayFormatter());
        }
        return "";
    }

    std::string MemberSelectorContent::GetScriptText() const
    {
        std::vector<std::string> args;
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (arg)
                args.push_back(arg->GetScriptText());
            else
                args.push_back("Argument " + std::to_string(i) + " is null");
        }
        return m_member->ToString(args);
    }

    bool MemberSelectorContent::IsContentEqual(const IContent& other) const
    {
        if (m_member->GetArity() == 0)
            return GetObject() == other.GetObject();
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            auto lhs = TryGetArgument(m_member, i);
            auto rhs = other.TryGetArgument(m_member, i);
            if (!lhs || !rhs)
                return false;
            if (!lhs->IsContentEqual(*rhs))
                return false;
        }
        return true;
    }

    bool MemberSelectorContent::IsValid() const
    {
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                return false;
            if (!arg->IsConvertibleTo(m_member->GetArgumentInfo(i)->GetTypeInfo()))
                return false;
        }
        return true;
    }

    Members::MemberPointer MemberSelectorContent::TryGetBestMember() const
    {
        return m_member;
    }

    Members::MemberPointer MemberSelectorContent::GetStoredMember() const
    {
        return m_member;
    }

    std::vector<std::string> MemberSelectorContent::DoErrorCheck() const
    {
        auto typeLibrary = Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
        auto typeFormatter = std::make_shared<TypeLibraries::TypeLibraryNamedTypeFormatter>(typeLibrary.get(), std::make_shared<Types::DefaultTypeFormatter>());
        for (size_t i = 0; i < m_member->GetArity(); ++i)
        {
            auto arg = TryGetArgument(m_member, i);
            if (!arg)
                return { "The " + std::to_string(i) + " argument of " + m_member->GetActualName() + " is not given" };
            if (!arg->IsConvertibleTo(m_member->GetArgumentInfo(i)->GetTypeInfo()))
                return { "The " + std::to_string(i) + " argument of " + m_member->GetActualName() + ": '" + arg->GetDisplayText() + "' + can not be converted to " + typeFormatter->FormatType(m_member->GetArgumentInfo(i)->GetTypeInfo()) };
        }
        return {};
    }

    std::shared_ptr<IContent> MemberSelectorContent::InvokeProperty(const std::string& propertyName) const
    {
        size_t index = Utilities::DefaultArgumentExtracter::GetIndexOfNamedConstructorArgument(m_member, propertyName);
        if (index < m_member->GetArity())
            return TryGetArgument(m_member, index);
        else
            return ContentUtility::InvokeProperty(*this, propertyName);
    }

    std::shared_ptr<IContent> MemberSelectorContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return InvokeProperty(propertyGetter->GetName());
    }

    std::shared_ptr<IContent> MemberSelectorContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent) const
    {
        if (typeInfo.GetTypeInfo() == m_member->GetReturnType().GetTypeInfo())
            return std::make_shared<MemberSelectorContent>(m_member);
        else
            return IContent::TryChangeType(typeInfo, existingContent);
    }

    bool MemberSelectorContent::HasScriptText() const
    {
        return false;
    }

    std::shared_ptr<IContent> MemberSelectorContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return MemberContent(member).TryGetArgument(member, index);
    }

    std::shared_ptr<IContent> MemberSelectorContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return ContentUtility::TryInvoke(this, member, arguments, trySimplify);
    }

    Objects::Object MemberSelectorContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return ContentUtility::ConvertTo(GetObject(), typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> MemberSelectorContent::Split() const
    {
        return { std::make_shared<MemberSelectorContent>(*this) };
    }

    bool MemberSelectorContent::IsCollection() const
    {
        return false;
    }

}}}}

