#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionCallbackAttribute.h"
#include "Reflection/Config.h"
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class UndefinedAttribute : public FunctionCallbackAttribute<bool> {
    public:
        template<typename UndefinedCallback>
        UndefinedAttribute(UndefinedCallback callback)
            :FunctionCallbackAttribute<bool>(callback, "")
        {
        }
    };
    bool REFLECTION_IMPORT_EXPORT IsUndefined(const Objects::Object& object);
}}}}