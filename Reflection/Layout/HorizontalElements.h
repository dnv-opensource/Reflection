#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CollectionOfElements.h"
#include "TypedLayoutElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class HorizontalElements : public TypedLayoutElement<HorizontalElements, CollectionOfElements>
    {
    public:
        HorizontalElements(HorizontalElements&& other) = default;
        HorizontalElements(const HorizontalElements& other) = default;
        HorizontalElements& operator=(HorizontalElements&& other) = default;
        HorizontalElements& operator=(const HorizontalElements& other) = default;

        template<typename... ElementsT>
        explicit HorizontalElements(ElementsT&&... args)
            : TypedLayoutElement<HorizontalElements, CollectionOfElements>(std::forward<ElementsT>(args)...)
        {
        }
    };
}}}}
