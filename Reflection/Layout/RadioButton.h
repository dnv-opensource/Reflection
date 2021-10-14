#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "AutoControl.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class RadioButton : public TypedLayoutElement<RadioButton, ControlElement>
    {
    public:
        RadioButton(const RadioButton&) = default;
        RadioButton(RadioButton&&) = default;
        RadioButton& operator=(const RadioButton&) = default;
        RadioButton& operator=(RadioButton&&) = default;

        RadioButton(const AutoControl& other)
            : TypedLayoutElement<RadioButton, ControlElement>(other)
        {}
        explicit RadioButton(const std::string& name = "")
            : TypedLayoutElement<RadioButton, ControlElement>(name, "")
        {}
        explicit RadioButton(const std::string& name, const std::string& caption)
            : TypedLayoutElement<RadioButton, ControlElement>(name, caption)
        {}
    };
}}}}
