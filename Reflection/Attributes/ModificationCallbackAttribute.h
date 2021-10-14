#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"
#include <vector>
#include "FunctionCallbackAttributeVararg.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    class ModificationCallbackAttribute : public FunctionCallbackAttribute<void()>
    {
    public:
        template<typename Callback, typename... Args>
        ModificationCallbackAttribute(const Callback& callback, Args&&... args)
            : Reflection::Attributes::FunctionCallbackAttribute<void()>(callback, std::forward<Args>(args)...)
        {}
    };

}}}}
