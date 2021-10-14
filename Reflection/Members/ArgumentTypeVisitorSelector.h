#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentTypeVisitor.h"
#include "ArgumentTypeVisitorVararg.h"
#include "TypeUtilities/FunctionTraits.h"
#include "Modifier.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    template<int Arity,bool IsVarargValue>
    class ArgumentTypeVisitorSelector : public ArgumentTypeVisitor
    {
    public:
        ArgumentTypeVisitorSelector(size_t i)
            : ArgumentTypeVisitor(i) {}
    };
    template<int Arity>
    class ArgumentTypeVisitorSelector<Arity,true> : public ArgumentTypeVisitorVararg<Arity-1>
    {
    public:
        ArgumentTypeVisitorSelector(size_t i)
            : ArgumentTypeVisitorVararg<Arity-1>(i) {}
    };
}}}}