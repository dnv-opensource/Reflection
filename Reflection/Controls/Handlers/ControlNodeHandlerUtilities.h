#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities/CompareCaseLess.h"
#include "Reflection/Controls/ControlNode.h"
#include <map>
#include "Reflection/Layout/ControlElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Given a set of nodes, such as "X","Y", and "Z" and a type, such as Vector3D, search the type library for a constructor that can be used to construct an object of that type given the input nodes and their types.
    Members::MemberPointer GetBestConstructor(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& info, const std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>& members);
    /*
    Tries to add the ConstructorAspect to a node. It also adds the FunctionArgumentAspect to the child nodes.
    This method will call GetBestConstructor internally.
    */
    void TryAddConstructor(ControlNode& parent, const std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>& members);

    void TryAddConstructorSelector(ControlNode& node);
    void TryAddMemberNode(ControlNode& node, const Members::MemberPointer& member);
    void TryAddFunctionArgumentNode(ControlNode& node, const Members::MemberPointer& member, size_t index);
    
    ControlNode* LookupRelatedNode(ControlNode& node, const std::shared_ptr<Layout::ControlElement>& element);
}}}}