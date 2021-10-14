#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include <string>
#include "IAttribute.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class AllowCreateAndModifyAttribute 
        : public FunctionCallbackAttribute<std::string>
        , public IAttribute
    {
    public:
        AllowCreateAndModifyAttribute()  {}
        template<typename CreateFunction, typename... StringArguments>
        AllowCreateAndModifyAttribute(CreateFunction function, const StringArguments&... stringArguments)
            : FunctionCallbackAttribute<std::string>(function, stringArguments...) {}
    };
}}}}