//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/FunctionArgumentAspect.h"
#include "Reflection/Controls/Aspects/ConstructorSelectorAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"
#include "Reflection/Controls/Aspects/SelectedConstructorAspect.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Utilities/MemberItem.h"
#include "Reflection/Attributes/IgnoreInViewAttribute.h"
#include "../../Attributes/AttributeCollectionService.h"
#include "boost/algorithm/string/split.hpp"
#include "../Aspects/TypeAspect.h"
#include "../../Attributes/ContextAttribute.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    using namespace Attributes;
    bool IsValidConstructor(const Members::MemberPointer& member, const std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>& members)
    {
        if (members.size() < member->GetMinimumArgumentCount())
            return false;
        if (members.size() > member->GetMaximumArgumentCount())
            return false;
        size_t argumentsFound = 0;
        for (size_t i = 0; i < member->GetArity(); ++i)
        {
            auto argumentInfo = member->GetArgumentInfo(i);
            if (!argumentInfo)
                return false;
            if (members.count(argumentInfo->GetName()) > 0)
                ++argumentsFound;
            else if (!argumentInfo->HasDefault())
                return false;
        }
        return argumentsFound == members.size();
    }

    Members::MemberPointer GetBestConstructor(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& info, const std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>& members)
    {
        auto constructors = Utilities::ConstructorsFromTypeService::GetService(typeLibrary)->CollectAllConstructorMembersOfThisAndDerivedClasses(info, false);
        for (const auto& constructor : constructors)
        {
            if (IsValidConstructor(constructor, members))
                return constructor;
        }
        return nullptr;
    }

    void TryAddConstructor(ControlNode& parent, const std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>& members)
    {
        if (!parent.GetTypeLibrary())
            return;
        auto constructor = GetBestConstructor(parent.GetTypeLibrary(), parent.GetDecoratedTypeInfo(), members);
        if (!constructor)
            return;
        parent.AddAspect<ConstructorAspect>(constructor);
        for (size_t i = 0; i < constructor->GetArity(); ++i)
        {
            auto argumentInfo = constructor->GetArgumentInfo(i);
            if (!argumentInfo)
                return;
            auto it = members.find(argumentInfo->GetName());
            if (it != members.end())
            {
                it->second->AddAspect<FunctionArgumentAspect>(constructor, i);
            }
        }
    }
    std::set<Members::MemberPointer> TryFilterContext(ControlNode& node, const std::set<Members::MemberPointer>& members)
    {
        const ContextAttribute* context = node.TryGetAttribute<ContextAttribute>();
        std::set<Members::MemberPointer> filtered;
        for (const Members::MemberPointer& member : members)
        {
            if (context)
            {
                if (const ContextAttribute* memberContext = GetPointerToAttributeOrNull<ContextAttribute>(member))
                {
                    if (!context->SharedContext(*memberContext))
                        continue;
                }
            }
            if(GetPointerToAttributeOrNull<IgnoreInViewAttribute>(member))
                continue;
            filtered.insert(member);
        }
        return filtered;
    }
    void TryAddConstructorSelector(ControlNode& node)
    {
        if (node.TryGetAspect<IsFunctionalNodeAspect>() || node.TryGetAspect<SelectedConstructorAspect>())
            return;
        if (node.GetChild("__SELECTOR__"))
            return;
        auto service = Utilities::ConstructorsFromTypeService::GetService(node.GetTypeLibrary());
        if (service)
        {
            bool isNested = false;
            ControlNode* parent = node.GetParent();
            while (parent)
            {
                if (const ConstructorSelectorAspect* aspect = parent->TryGetAspect<ConstructorSelectorAspect>())
                {
                    if (aspect && aspect->GetDecoratedTypeInfo() == node.GetDecoratedTypeInfo())
                    {
                        isNested = true;
                        break;
                    }
                }
                parent = parent->GetParent();
            }
            auto constructors = TryFilterContext(node, service->CollectAllConstructorMembersOfThisAndDerivedClasses(node.GetDecoratedTypeInfo(), !isNested));
            auto selectorNode = std::make_shared<ControlNode>(&node);
            selectorNode->SetDecoratedTypeInfo(Types::TypeId<Utilities::MemberItem>());
            selectorNode->AddAspect<ConstructorSelectorAspect>(node.GetDecoratedTypeInfo(), constructors);
            selectorNode->SetName("__SELECTOR__");
            selectorNode->SetCaption("");
            node.AddChild(selectorNode);
        }

    }

    void TryAddMemberNode(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!member)
            return;
        if (node.LookupRelatedNode(member->GetName()))
            return;
        if (GetPointerToAttributeOrNull<IgnoreInViewAttribute>(member))
            return;
        auto result = std::make_shared<ControlNode>(&node, member);
        if (result->SupportAssign())
            node.AddChild(result);
    }

    void TryAddFunctionArgumentNode(ControlNode& node, const Members::MemberPointer& member, size_t index)
    {
        if (!node.GetTypeLibrary())
            return;
        auto info = member->GetArgumentInfo(index);
        if (!info || info->GetName().empty())
            return;
        if (auto child = node.LookupRelatedNode(info->GetName()))
            return child->AddAspect<FunctionArgumentAspect>(member, index);
        auto child = std::make_shared<ControlNode>(&node, info->GetTypeInfo());
        child->SetName(info->GetName());
        if (TypeLibraries::TypePointer type = node.GetTypeLibrary()->LookupType(node.GetDecoratedTypeInfo().GetTypeInfo()))
            node.GetManager()->OnInitializeMember(*child, type->Lookup(info->GetName(), Variants::Variant()));
        child->AddAspect<FunctionArgumentAspect>(member, index);
        node.AddChild(child);
    }

    ControlNode* LookupRelatedNode(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element)
    {
        if (!element)
            return nullptr;
        if (element->GetName().empty())
            return &node;
        if (_strcmpi(element->GetName().c_str(), node.GetName().c_str()) == 0)
            return &node;
        return node.LookupRelatedNode(element->GetName(), 0, 1).get();
    }

    std::shared_ptr<ControlNode> GetOrCreateNestedNode(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element)
    {
        std::vector<std::string> nested;
        boost::algorithm::split(nested, element->GetName(), [](char c) {return c == '.'; });
        ControlNode* current = node.GetParent();
        std::shared_ptr<ControlNode> nestedNode;
        for (size_t i = 0; i < nested.size(); ++i)
        {
            if (!current)
                return nullptr;
            const std::string& name = nested[i];
            if (current->HasChild(name))
                nestedNode = current->GetChild(name);
            else
            {
                if (i == 0)
                    return nullptr;
                auto member = current->GetAspect<TypeAspect>().GetType()->Lookup(name, Variants::Variant());
                if (member)
                {
                    auto child = std::make_shared<ControlNode>(current, member);
                    child->SetName(name);
                    current->AddChild(child);
                    nestedNode = child;
                }
            }
            if (nestedNode)
                current = nestedNode.get();
        }
        return nestedNode;
    }

}}}}

