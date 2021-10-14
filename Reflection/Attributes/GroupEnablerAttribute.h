#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "FunctionCallbackAttribute.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    //Add this attribute to a member in order to replace the caption of the group with a check box to enable/disable all items under that group.
    class GroupEnablerAttribute 
        : public FunctionCallbackAttribute<bool>
        , public IAttribute
    {
    public:
        GroupEnablerAttribute() {}
        template<typename GetterT, typename... StringArguments>
        GroupEnablerAttribute(const GetterT& getter, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<bool>(getter, stringArguments...) {}

        bool IsChecked(const std::vector<Objects::Object>& arguments) const
        {
            if(HasFunction())
                return InvokeFunction(arguments);
            else 
                return false;
        }
    };
}}}}
