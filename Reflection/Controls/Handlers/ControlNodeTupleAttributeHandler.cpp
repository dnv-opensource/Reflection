//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeTupleAttributeHandler.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Controls/Aspects/ConstructorAspect.h"
#include "Reflection/Controls/Aspects/FunctionArgumentAspect.h"
#include "Reflection/Controls/Aspects/IndexedPropertyAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

	bool ControlNodeTupleAttributeHandler::OnInitialize(ControlNode& node, const Attributes::TupleAttribute& attribute, bool priorState)
	{
		TypeLibraries::TypePointer tupleType = node.GetTypeLibrary()->LookupType(attribute.GetTupleType().GetTypeInfo());
		if (!tupleType)
			return priorState;
        std::map<size_t, std::shared_ptr<ControlNode>> indexedChildren;
        for (const std::shared_ptr<ControlNode>& child : node.GetChildren())
        {
            if (const FunctionArgumentAspect* aspect = child->TryGetAspect<FunctionArgumentAspect>())
                indexedChildren.emplace(aspect->GetIndex(), child);
        }
		auto getter = tupleType->Lookup("Get");
		auto setter = tupleType->Lookup("Set");
		Members::MemberPointer constructor;
		Members::MemberPointer constructors = tupleType->Lookup(Members::ConstructorString);
		if (!constructors)
			return priorState;
		for (const auto& overload : constructors->GetOverloads())
		{
			if (overload->GetArity() == attribute.GetArgumentTypes().size())
			{
				constructor = overload;
				break;
			}
		}
		if (!constructor || !getter || !setter)
			return priorState;
		node.AddAspect<ConstructorAspect>(constructor);
		for (size_t i = 0; i < attribute.GetArgumentTypes().size(); ++i)
		{
            std::shared_ptr<ControlNode> arg;
            auto it = indexedChildren.find(i);
            if (it != indexedChildren.end())
                arg = it->second;
            else
            {
                arg = std::make_shared<ControlNode>(&node, attribute.GetArgumentTypes()[i]);
                arg->AddAspect<FunctionArgumentAspect>(constructor, i);
            }
			IndexedPropertyAspect::IndexMap indices{ { "index", Objects::Object(node.GetTypeLibrary(), i) } };
			arg->AddAspect<IndexedPropertyAspect>(setter, getter, indices);
			arg->SetName(std::to_string(i));
			if(it == indexedChildren.end())
                node.AddChild(arg);
		}
		return true;
	}

}}}}