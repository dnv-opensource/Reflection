//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionContent.h"
#include "Reflection/Members/IMember.h"
#include "Impl/ContentUtility.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Contents { 

    CollectionContent::CollectionContent(const std::vector<std::shared_ptr<IContent>>& contentVector)
        : m_contentVector(contentVector)
    {
    }

    bool CollectionContent::IsConvertibleTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        for (const auto& content : m_contentVector)
        {
            if (!content->IsConvertibleTo(typeInfo))
                return false;
        }
        return !m_contentVector.empty();
    }

    Types::DecoratedTypeInfo CollectionContent::GetTypeInfo() const
    {
        if (m_contentVector.empty())
            return Types::DecoratedTypeInfo();
        else
            return m_contentVector.front()->GetTypeInfo();
    }

    Objects::Object CollectionContent::GetObject() const
    {
        return Objects::Object();
    }

    bool CollectionContent::HasStoredObject() const
    {
        return false;
    }

    std::string CollectionContent::GetDisplayText() const
    {
        return "*";
    }

    std::string CollectionContent::GetScriptText() const
    {
        return "*";
    }

    bool CollectionContent::HasScriptText() const
    {
        return false;
    }

    bool CollectionContent::IsTextEqual(const IContent& other) const
    {
        return false;
    }

    bool CollectionContent::IsContentEqual(const IContent& other) const
    {
        return false;
    }

    bool CollectionContent::IsValid() const
    {
        for (const auto& content : m_contentVector)
            if (!content->IsValid())
                return false;
        return true;
    }

    bool CollectionContent::HasMetaData() const
    {
        return true;
    }

    std::shared_ptr<IContent> CollectionContent::TryGetArgument(const Members::MemberPointer& member, size_t index) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->TryGetArgument(member, index); });
    }

    std::shared_ptr<Contents::IContent> CollectionContent::TryChangeType(const Types::DecoratedTypeInfo& typeInfo, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->TryChangeType(typeInfo, existingContent); });
    }

    std::shared_ptr<Contents::IContent> CollectionContent::TryChangeMember(const Members::MemberPointer& member, const std::shared_ptr<IContent>& existingContent /*= nullptr*/) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->TryChangeMember(member, existingContent); });
    }

    Members::MemberPointer CollectionContent::TryGetBestMember() const
    {
        std::set<Members::MemberPointer> members;
        for (const auto& content : m_contentVector)
        {
            auto member = content->TryGetBestMember();
            if (member)
                members.insert(member);
        }
        if (members.size() == 1)
            return *members.begin();
        return nullptr;
    }

    Members::MemberPointer CollectionContent::GetStoredMember() const
    {
        std::set<Members::MemberPointer> members;
        for (const auto& content : m_contentVector)
        {
            auto member = content->GetStoredMember();
            if (member)
                members.insert(member);
        }
        if (members.size() == 1)
            return *members.begin();
        return nullptr;
    }

    std::vector<std::string> CollectionContent::DoErrorCheck() const
    {
        return {};
    }

    std::shared_ptr<IContent> CollectionContent::InvokeProperty(const std::string& propertyName) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->InvokeProperty(propertyName); });
    }

    std::shared_ptr<IContent> CollectionContent::InvokeProperty(const Members::MemberPointer& propertyGetter) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->InvokeProperty(propertyGetter); });
    }

    std::shared_ptr<IContent> CollectionContent::TryInvoke(const Members::MemberPointer& member, const std::vector<std::shared_ptr<IContent>>& arguments, bool trySimplify) const
    {
        return Transform([&](const std::shared_ptr<IContent>& content) {return content->TryInvoke(member, arguments, trySimplify); });
    }

    Objects::Object CollectionContent::GetObjectConvertTo(const Types::DecoratedTypeInfo& typeInfo) const
    {
        if (m_contentVector.empty())
            return Objects::Object();
        auto container = ContentUtility::TryCreateContainer(m_contentVector.front()->GetObject().GetTypeLibrary(), typeInfo);
        for (const auto& content : m_contentVector)
        {
            if (!ContentUtility::TryAddItemToContainer(container, content->GetObject()))
                return Objects::Object();
        }
        return container;
    }

    std::vector<std::shared_ptr<IContent>> CollectionContent::Split() const
    {
        std::vector<std::shared_ptr<IContent>> result;
        for (const auto& content : m_contentVector)
        {
            for (const auto& splitContent : content->Split())
                result.push_back(content);
        }
        return result;
    }

    bool CollectionContent::IsCollection() const
    {
        return true;
    }

    std::shared_ptr<IContent> CollectionContent::Transform(const std::function<std::shared_ptr<IContent>(const std::shared_ptr<IContent>& content)>& algorithm) const
    {
        std::vector<std::shared_ptr<IContent>> result;
        for (const auto& content : m_contentVector)
        {
            auto argument = algorithm(content);
            if (argument)
            {
                if (result.empty() || !result.back()->IsContentEqual(*argument))
                    result.push_back(argument);
            }
        }
        if (result.size() == 1)
            return result.back();
        else if (result.empty())
            return nullptr;
        else
            return std::make_shared<CollectionContent>(result);
    }

} } } }

