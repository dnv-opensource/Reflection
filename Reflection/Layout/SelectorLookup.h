#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLayoutElement.h"
#include "ControlElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class SelectorLookup : public TypedLayoutElement<SelectorLookup, ControlElement>
    {
    public:
        SelectorLookup(const SelectorLookup&) = default;
        SelectorLookup(SelectorLookup&&) = default;
        SelectorLookup& operator=(const SelectorLookup&) = default;
        SelectorLookup& operator=(SelectorLookup&&) = default;

        SelectorLookup(const std::string& name) : TypedLayoutElement<SelectorLookup, ControlElement>(name) {}
    };
}}}}
