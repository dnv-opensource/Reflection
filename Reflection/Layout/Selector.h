#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLayoutElement.h"
#include "ElementWithOneChild.h"
#include "Reflection/Utilities/MemberItem.h"
#include "ControlElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Selector : public TypedLayoutElement<Selector, ElementWithOneChild>
    {
    public:
        //Needed in unit tests
        Selector() {}
        Selector(const Selector& other) = default;
        Selector(Selector&& other) = default;
        Selector& operator=(const Selector& other) = default;
        Selector& operator=(Selector&& other) = default;

        template<typename T>
        Selector(const T& element, bool setCaptionEqualsName = true, std::enable_if_t<std::is_base_of_v<ControlElement, T>, void*> = nullptr) : Selector(std::make_shared<T>(element), setCaptionEqualsName)
        {}
        template<typename T>
        Selector(const std::shared_ptr<T>& element, bool setCaptionEqualsName = true, std::enable_if_t<std::is_base_of_v<ControlElement, T>, void*> = nullptr) : TypedLayoutElement<Selector, ElementWithOneChild>(element)
        {
            element->GetContext().type = Types::TypeId<Utilities::MemberItem>();
            if(setCaptionEqualsName)
                element->SetCaption(element->GetName());
        }
    };
}}}}
