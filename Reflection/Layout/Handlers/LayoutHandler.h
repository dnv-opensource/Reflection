#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Layout/ElementWithOneChild.h"
#include "Reflection/Layout/CollectionOfElements.h"
#include "ILayoutHandler.h"
#include "TypeUtilities/TypeList.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {    
    template<typename DerivedT, typename... Types>
    class LayoutHandler : public ILayoutHandler {
    public:
        using BaseClass = LayoutHandler<DerivedT, Types...>;
        DerivedT& GetDerived() {
            return static_cast<DerivedT&>(*this);
        }
        std::shared_ptr<ILayoutElement> ModifyLayout(std::shared_ptr<ILayoutElement> element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context) override
        {
            return ModifyLayoutImpl(TypeUtilities::TypeList<Types..., ElementWithOneChild, CollectionOfElements>(), element, controlNode, context);
        }
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<ElementWithOneChild>& element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
        {
            element->SetChild(ModifyLayout(element->GetChild(), controlNode, context));
            return element;
        }
        std::shared_ptr<ILayoutElement> ModifyLayout(const std::shared_ptr<CollectionOfElements>& element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
        {
            for (auto it = element->begin(); it != element->end(); ++it)
                *it = ModifyLayout(*it, controlNode, context);
            return element;
        }
    private:
        std::shared_ptr<ILayoutElement> ModifyLayoutImpl(TypeUtilities::TypeList<>, const std::shared_ptr<ILayoutElement>& element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
        {
            return element;
        }
        template<typename T, typename... Remainder>
        std::shared_ptr<ILayoutElement> ModifyLayoutImpl(TypeUtilities::TypeList<T, Remainder...>, const std::shared_ptr<ILayoutElement>& element, const std::shared_ptr<Controls::ControlNode>& controlNode, ILayoutControlContext& context)
        {
            auto concreteElement = std::dynamic_pointer_cast<T>(element);
            if (concreteElement)
                return GetDerived().ModifyLayout(concreteElement, controlNode, context);
            else
                return ModifyLayoutImpl(TypeUtilities::TypeList<Remainder...>(), element, controlNode, context);
        }
    };
}}}}