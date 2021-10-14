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
    Represents rules on whether a field is to be enabled or not.
    First comes a function with the rules for the enabling/disabling
    Then comes a list of property names that should be passed to this rule function.
    */

    class EnabledAttribute 
        : public FunctionCallbackAttribute<bool>
        , public IAttribute
    {
    public:
        EnabledAttribute(bool enabled) : FunctionCallbackAttribute<bool>(enabled) {}
        template<typename EnableControlFunction, typename... StringArguments>
        EnabledAttribute(const Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, EnableControlFunction enableControlFunction, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<bool>(typeLibrary, enableControlFunction, stringArguments...) {}
        template<typename EnableControlFunction, typename... StringArguments>
        EnabledAttribute(EnableControlFunction enableControlFunction, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<bool>(enableControlFunction, stringArguments...) {}
        bool IsEnabled(const std::vector<Objects::Object>& arguments) const
        {
            return InvokeFunction(arguments);
        }
    };
}}}}