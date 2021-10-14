//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeFunctionSelectorAttributeHandler.h"
#include "Reflection/Types/TypeFormatter.h"
#include "Reflection/Controls/Aspects/FunctionSelectorAspect.h"
#include "Reflection/Controls/Aspects/IsFunctionalNodeAspect.h"
#include "Reflection/Utilities/MemberItem.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeFunctionSelectorAttributeHandler::OnInitialize(ControlNode& node, const Attributes::FunctionSelectorAttribute& attribute, bool priorState)
    {
        if (!node.GetTypeLibrary())
            return priorState;
        auto type = node.GetTypeLibrary()->LookupType(node.GetDecoratedTypeInfo().GetTypeInfo());
        if (!type)
            throw std::runtime_error("Type " + Types::TypeFormatter().FormatType(node.GetDecoratedTypeInfo()) + " not reflected");        
        std::list<Members::MemberPointer> members;
        std::set<std::string> existingMembers;
        for (const auto& memberName : attribute.GetMemberNames())
        {
            if (!existingMembers.insert(memberName).second)
                continue;
            auto member = type->Lookup(memberName, Variants::Variant());
            if (!member)
                throw std::runtime_error("Member " + memberName + " not found on type " + Types::TypeFormatter().FormatType(node.GetDecoratedTypeInfo()));
            for (const auto& overload : member->GetOverloads())
                members.push_back(overload);
        }
        if (!members.empty())
        {
            node.SetCaption(node.GetName());
            node.SetName("__this__");
            auto selectorNode = std::make_shared<ControlNode>(&node);
            selectorNode->SetDecoratedTypeInfo(Types::TypeId<Utilities::MemberItem>());
            selectorNode->AddAspect<FunctionSelectorAspect>(members);
            selectorNode->SetName("__SELECTOR__");
            selectorNode->SetCaption("");
            node.AddChild(selectorNode);
            node.AddAspect<IsFunctionalNodeAspect>();
        }
        return priorState;
    }

}}}}

