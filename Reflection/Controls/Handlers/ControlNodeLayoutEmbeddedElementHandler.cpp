//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeLayoutEmbeddedElementHandler.h"
#include "ControlNodeHandlerUtilities.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Utilities/ConstructorsFromTypeService.h"
#include "Reflection/Utilities/ConstructorsFromTypeExtracter.h"
#include "Reflection/TypeLibraries/TypeWithBases.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    bool ControlNodeLayoutEmbeddedElementHandler::OnInitialize(ControlNode& node, const std::shared_ptr<Layout::EmbeddedElement>& element, bool priorState)
    {
        if (!element->GetChild() && node.TryGetAspect<ConstructorAspect>() == nullptr)
        {
            //Find constructor 
            auto service = Utilities::ConstructorsFromTypeService::GetService(node.GetTypeLibrary());
            auto constructor = Utilities::ConstructorsFromTypeExtracter(node.GetTypeLibrary()).GetBestMemberFromType(service->CollectAllConstructorMembersOfThisAndDerivedClasses(node.GetDecoratedTypeInfo(), true), node.GetDecoratedTypeInfo());
            if (constructor)
                node.GetManager()->OnInitializeMember(node, constructor);
            else
            {
                if (auto type = node.GetAspect<TypeAspect>().GetType())
                {
                    TypeLibraries::TypeWithBases wrapperType(type, node.GetTypeLibrary());
                    for (const auto& memberName : wrapperType.GetUnsortedMembers())
                    {
                        auto member = wrapperType.Lookup(memberName, Variants::Variant());
                        if(member->GetMemberType() != Members::MemberType::TypeConstructor)
                            TryAddMemberNode(node, wrapperType.Lookup(memberName, Variants::Variant()));
                    }
                }
            }
        }
        OnInitialize(node, element->GetChild(), priorState);
        return priorState;
    }

}}}}

