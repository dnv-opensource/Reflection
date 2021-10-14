//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNode.h"
#include "Reflection/Types/DecoratedTypeFormatter.h"
#include "Reflection/Types/TypeFormatter.h"
#include "ControlNodeManager.h"
#include "Aspects/CustomAttributeCollectionAspect.h"
#include "Aspects/TypeAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    
    bool ControlNode::operator==(const ControlNode& other) const
    {
        if (GetName() != other.GetName())
            return false;
        if (GetDecoratedTypeInfo() != other.GetDecoratedTypeInfo())
            return false;
        if (m_aspects.size() != other.m_aspects.size())
            return false;
        auto ithis = m_aspects.begin();
        auto iother = other.m_aspects.begin();
        for (; ithis != m_aspects.end(); ++ithis, ++iother)
        {
            if (**ithis != **iother)
                return false;
        }
        if (m_children.size() != other.m_children.size())
            return false;
        auto ileft = m_children.begin();
        auto iright = other.m_children.begin();
        for (;ileft != m_children.end();++ileft,++iright)
        {
            if (*ileft != *iright)
                return false;
        }
        return true;
    }

    bool ControlNode::operator!=(const ControlNode& other) const
    {
        return !operator==(other);
    }

    ControlNode::ControlNode(const std::shared_ptr<ControlNodeManager>& manager)
        : m_manager(manager)
        , m_parent(nullptr)
        , m_caption("__USENAME__")
    {
    }

    ControlNode::ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& decoratedTypeInfo, Detail::NoInit)
        : ControlNode(typeLibrary)
    {
        if (m_manager)
        {
            m_manager->OnInitializeType(*this, decoratedTypeInfo);
        }
    }

    ControlNode::ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, Detail::NoInit)
        : ControlNode(typeLibrary)
    {
        if (m_manager)
        {
            m_manager->OnInitializeMember(*this, member);
        }        
    }

    void ControlNode::InitializeType(const Types::DecoratedTypeInfo& decoratedTypeInfo, bool finalizeInitialization)
    {
        if (m_manager)
        {
            m_manager->OnInitializeType(*this, decoratedTypeInfo);
			if(finalizeInitialization)
				m_manager->OnInitialize(*this);
        }
    }

    void ControlNode::InitializeMember(const Members::MemberPointer& member)
    {
        if (m_manager)
        {
            m_manager->OnInitializeMember(*this, member);
            m_manager->OnInitialize(*this);
        }
    }

    void ControlNode::Initialize()
    {
        if (m_manager)
            m_manager->OnInitialize(*this);
    }

    ControlNode::ControlNode(const std::shared_ptr<ControlNodeManager>& manager, const Types::DecoratedTypeInfo& decoratedTypeInfo)
        : ControlNode(manager)
    {
        InitializeType(decoratedTypeInfo);
    }

    ControlNode::ControlNode(const std::shared_ptr<ControlNodeManager>& manager, const Members::MemberPointer& member)
        : ControlNode(manager)
    {
        InitializeMember(member);
    }

    ControlNode::ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary)
        : ControlNode(std::make_shared<ControlNodeManager>(typeLibrary))
    {

    }

    ControlNode::ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& decoratedTypeInfo)
        : ControlNode(std::make_shared<ControlNodeManager>(typeLibrary), decoratedTypeInfo)
    {
    }

    ControlNode::ControlNode(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member)
        : ControlNode(std::make_shared<ControlNodeManager>(typeLibrary), member)
    {
    }

    ControlNode::ControlNode(ControlNode* parent)
        : m_parent(parent)
        , m_manager(parent->GetManager())
        , m_caption("__USENAME__")
    {
    }

    ControlNode::ControlNode(ControlNode* parent, const Members::MemberPointer& member)
        : ControlNode(parent)
    {
        if (m_manager)
            m_manager->OnInitializeMember(*this, member);
    }

    ControlNode::ControlNode(ControlNode* parent, const Types::DecoratedTypeInfo& decoratedTypeInfo)
        : ControlNode(parent)
    {
        if (m_manager)
            m_manager->OnInitializeType(*this, decoratedTypeInfo);
    }

    std::string ControlNode::ToString() const
    {
        std::string text = "ControlNode(";
        std::list<std::string> arguments;
        if (GetDecoratedTypeInfo() != Types::DecoratedTypeInfo())
            arguments.emplace_back("TypeId<" + Types::DecoratedTypeFormatter(std::make_shared<Types::TypeFormatter>()).FormatType(GetDecoratedTypeInfo()) + ">");
        if (!GetName().empty())
            arguments.emplace_back("\"" + m_name + "\"");
        for (const auto& aspect : m_aspects)
        {
            arguments.emplace_back(aspect->ToString());
        }
        bool addComma = false;
        for (const std::string& arg : arguments)
        {
            if (addComma)
                text += ", ";
            else
                addComma = true;
            text += arg;
        }
        text += ")";
        return text;
    }

    const Types::DecoratedTypeInfo& ControlNode::GetDecoratedTypeInfo() const
    {
        return m_decoratedTypeInfo;
    }

    void ControlNode::SetDecoratedTypeInfo(const Types::DecoratedTypeInfo& val)
    {
        m_decoratedTypeInfo = val;
    }

    void ControlNode::SetName(const std::string& val)
    {
        m_name = val;
    }

    TypeLibraries::TypeLibraryPointer ControlNode::GetTypeLibrary() const
    {
        if (m_manager)
            return m_manager->GetTypeLibrary();
        return nullptr;
    }

    void ControlNode::AddChild(std::shared_ptr<ControlNode> node)
    {
        if (node && node->GetManager())
        {
            m_children.push_back(node);
            node->GetManager()->OnInitialize(*node);
        }
    }

    bool ControlNode::HasChild(const std::string& name) const
    {
        return GetChild(name) != nullptr;
    }

    std::shared_ptr<ControlNode> ControlNode::GetChild(const std::string& name) const
    {
        for (const auto& child : m_children)
        {
            if (_strcmpi(child->GetName().c_str(), name.c_str()) == 0)
                return child;
        }
        return nullptr;
    }

    std::shared_ptr<ControlNode> ControlNode::LookupRelatedNode(const std::string& name, int recursionLevelParent, int recursionLevelChild) const
    {
        for (const auto& child : m_children)
        {
            if (_strcmpi(child->GetName().c_str(), name.c_str()) == 0)
                return child;
            if (recursionLevelChild > 0)
            {
                if (auto related = child->LookupRelatedNode(name, 0, recursionLevelChild - 1))
                    return related;
            }
        }
        if (recursionLevelParent > 0 && m_parent)
            return m_parent->LookupRelatedNode(name, recursionLevelParent - 1, 0);
        return nullptr;
    }

    std::pair<ControlNode*, Members::MemberPointer> ControlNode::LookupRelatedMember(const std::string& name, int recursionLevelParent, int recursionLevelChild)
    {
        if (auto typeAspect = TryGetAspect<TypeAspect>())
        {
            if (typeAspect->GetType())
            {
                if (auto member = typeAspect->GetType()->Lookup(name, Variants::Variant()))
                    return std::make_pair(this, member);
            }
        }
        if (recursionLevelChild > 0)
        {
            for (const auto& child : m_children)
            {
                auto memberPair = child->LookupRelatedMember(name, 0, recursionLevelChild-1);
                if (memberPair.first)
                    return memberPair;
            }
        }
        if (recursionLevelParent > 0 && m_parent)
            return m_parent->LookupRelatedMember(name, recursionLevelParent-1, 0);
        return std::make_pair(nullptr, nullptr);
    }

    Contents::Content ControlNode::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify) const
    {
        for (const auto& aspect : m_aspects)
        {
            Contents::Content result = aspect->TryGetPropertyContent(parent, trySimplify);
            if (result)
                return result;
        }
        return Contents::Content();
    }

    Contents::Content ControlNode::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        for (const auto& aspect : m_aspects)
        {
            Contents::Content result = aspect->TrySetPropertyContent(parent, value);
            if (result)
                return result;
        }
        return Contents::Content();
    }

    bool ControlNode::SupportUpdateParent() const
    {
        for (const auto& aspect : m_aspects)
        {
            if(aspect->SupportUpdateParent())
                return true;
        }
        return false;
    }

    bool ControlNode::SupportAssign() const
    {
        for (const auto& aspect : m_aspects)
        {
            if (aspect->SupportAssign())
                return true;
        }
        return false;
    }

    bool ControlNode::HasMember() const
    {
        for (const auto& aspect : m_aspects)
        {
            if (aspect->HasMember())
                return true;
        }
        return false;
    }

    void ControlNode::SetCustomAttributeCollection(Attributes::AttributeCollection&& attributeCollection)
    {
        AddAspect<CustomAttributeCollectionAspect>(std::move(attributeCollection));
    }

    std::string ControlNode::GetCaption() const
    {
        if (HasCaption())
            return m_caption;
        return m_name;
    }

    bool ControlNode::HasCaption() const
    {
        return m_caption != "__USENAME__";
    }

    void ControlNode::SetCaption(const std::string& caption)
    {
        m_caption = caption;
    }

    const std::shared_ptr<ControlNodeManager>& ControlNode::GetManager() const
    {
        return m_manager;
    }

    ControlNode* ControlNode::GetParent() const
    {
        return m_parent;
    }

    void ControlNode::SetParent(ControlNode* node)
    {
        m_parent = node;
    }

}}}}
