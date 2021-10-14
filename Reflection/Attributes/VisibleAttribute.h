#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Represents rules on whether a field is to be visible or not.
    First comes a function with the rules for the visible/invisible
    Then comes a list of property names that should be passed to this rule function.
    */

    class VisibleAttribute 
        : public FunctionCallbackAttribute<bool>
        , public IAttribute
    {
    public:
        VisibleAttribute(bool visible) : FunctionCallbackAttribute<bool>(visible) {}
        template<typename VisibleControlFunction, typename... StringArguments>
        VisibleAttribute(VisibleControlFunction visibleControlFunction, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<bool>(visibleControlFunction, stringArguments...) {}
        bool IsVisible(const std::vector<Objects::Object>& arguments) const
        {
            return InvokeFunction(arguments);
        }
    };
}}}}