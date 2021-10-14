//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "AssignmentContent.h"
#include "Reflection/Members/IMember.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Contents { 

    AssignmentContent::AssignmentContent(const std::shared_ptr<IContent>& lhs, const std::shared_ptr<IContent>& rhs)
        : m_lhs(lhs)
        , m_rhs(rhs)
    {

    }

    bool AssignmentContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        if (m_lhs)
            return m_lhs->IsConvertibleTo(typeInfo);
        else
            return false;
    }

    Types::DecoratedTypeInfo AssignmentContent::GetTypeInfo() const
    {
        if (m_lhs)
            return m_lhs->GetTypeInfo();
        else
            return Types::DecoratedTypeInfo();
    }

    Objects::Object AssignmentContent::GetObject() const
    {
        if (m_lhs && m_rhs)
        {
            auto lhsObject = m_lhs->GetObject();
            auto rhsObject = m_rhs->GetObject();
            if (rhsObject.IsValid() && lhsObject.IsValid())
            {
                lhsObject.Assign(rhsObject);
                return lhsObject;
            }
            return rhsObject;
        }
        return Objects::Object();
    }

    bool AssignmentContent::HasStoredObject() const
    {
        return false;
    }

    std::string AssignmentContent::GetDisplayText() const
    {
        return GetScriptText();
    }

    std::string AssignmentContent::GetScriptText() const
    {
        if (m_lhs && m_rhs)
            return m_lhs->GetScriptText() + " = " + m_rhs->GetScriptText();
        else
            return "";
    }

    bool AssignmentContent::HasScriptText() const
    {
        return true;
    }

    bool AssignmentContent::IsTextEqual(const IContent& other) const
    {
        return GetScriptText() == other.GetScriptText();
    }

    bool AssignmentContent::IsContentEqual(const IContent& other) const
    {
        return false;
    }

    bool AssignmentContent::IsValid() const
    {
        return m_lhs && m_rhs && m_rhs->IsValid();
    }

    bool AssignmentContent::HasMetaData() const
    {
        if (m_lhs && m_rhs)
            return m_lhs->HasMetaData() || m_rhs->HasMetaData();
        return false;
    }

    std::shared_ptr<IContent> AssignmentContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return nullptr;
    }

    std::shared_ptr<IContent> AssignmentContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        return nullptr;
    }

    std::shared_ptr<IContent> AssignmentContent::TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        return nullptr;
    }

    Members::MemberPointer AssignmentContent::TryGetBestMember() const
    {
        return nullptr;
    }

    Members::MemberPointer AssignmentContent::GetStoredMember() const
    {
        return nullptr;
    }

    std::vector<std::string> AssignmentContent::DoErrorCheck() const
    {
        if (m_lhs && m_rhs)
            return m_rhs->DoErrorCheck();
        else
        {
            return { "Arguments of assignment cannot be null" };
        }
    }

    std::shared_ptr<IContent> AssignmentContent::InvokeProperty(const std::string& propertyName) const
    {
        return nullptr;
    }

    std::shared_ptr<IContent> AssignmentContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return nullptr;
    }

    std::shared_ptr<IContent> AssignmentContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return nullptr;
    }

    Objects::Object AssignmentContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        return GetObject().ConvertTo(typeInfo);
    }

    std::vector<std::shared_ptr<IContent>> AssignmentContent::Split() const
    {
        return {};
    }

    bool AssignmentContent::IsCollection() const
    {
        return false;
    }

}}}}

