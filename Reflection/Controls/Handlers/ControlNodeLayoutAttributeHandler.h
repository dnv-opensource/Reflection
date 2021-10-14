#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlNodeAttributeHandler.h"
#include "Reflection/Attributes/LayoutAttribute.h"
#include "Reflection/Layout/ElementWithOneChild.h"
#include "Reflection/Layout/CollectionOfElements.h"
#include "TypeUtilities/TypeList.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    /*
    This is the base class handler for the LayoutAttribute. It will interpret the main branching elements and leave the leaf node handling to specialized handlers.
    */
    template<typename DerivedT, typename... Types>
    class ControlNodeLayoutAttributeHandler : public IControlNodeAttributeHandler<Attributes::LayoutAttribute>
    {
    public:
        using BaseClass = ControlNodeLayoutAttributeHandler<DerivedT, Types...>;
        DerivedT& GetDerived() {
            return static_cast<DerivedT&>(*this);
        }
        bool OnInitialize(ControlNode& node, const Attributes::LayoutAttribute& attribute, bool priorState = false) override
        {
            return GetDerived().OnInitialize(node, attribute.GetLayout(), priorState);
        }
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ILayoutElement>& layoutElement, bool priorState = false)
        {
            //Non-intrtusive "visitor" pattern. OnInitializeImpl will dynamic_cast layoutElement to all the types in the TypeList and call the corresponding OnInitialize overload for that type.
            //If no overload is found, no OnInitialize method will be called.
            return OnInitializeImpl(TypeUtilities::TypeList<Types..., Layout::ElementWithOneChild, Layout::CollectionOfElements>(), node, layoutElement, priorState);
        }
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::ElementWithOneChild>& element, bool priorState = false)
        {
            return OnInitialize(node, element->GetChild(), priorState);
        }
        bool OnInitialize(ControlNode& node, const std::shared_ptr<Layout::CollectionOfElements>& element, bool priorState = false)
        {
            for (auto it = element->begin(); it != element->end(); ++it)
                priorState = OnInitialize(node, *it, priorState);
            return priorState;
        }
    private:
        bool OnInitializeImpl(TypeUtilities::TypeList<>, ControlNode& node, const std::shared_ptr<Layout::ILayoutElement>& layoutElement, bool priorState = false)
        {
            return priorState;
        }
        template<typename T, typename... Remainder>
        bool OnInitializeImpl(TypeUtilities::TypeList<T, Remainder...>, ControlNode& node, const std::shared_ptr<Layout::ILayoutElement>& layoutElement, bool priorState = false)
        {
            //Try the first type in the list and cast to it. If it matches, initialize the node. Otherwise, try the next type.
            auto concreteElement = std::dynamic_pointer_cast<T>(layoutElement);
            if (concreteElement)
                return GetDerived().OnInitialize(node, concreteElement, priorState);
            else
                return OnInitializeImpl(TypeUtilities::TypeList<Remainder...>(), node, layoutElement, priorState);
        }
    };
}}}}