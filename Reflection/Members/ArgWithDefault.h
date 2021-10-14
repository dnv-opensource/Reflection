#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "ArgumentList.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    ///Defines an argument in a signature with a default value. Implicitly created from Arg(...)=value.
    class ArgWithDefault : public NamedArgumentListWithDefaults<1>
    {
    public:
        ArgWithDefault(ArgumentInfoPointer argument,const Variants::Variant& inDefault) : NamedArgumentListWithDefaults<1>(argument) {
            at(0)->SetDefault(inDefault);
        }
    };

}}}}