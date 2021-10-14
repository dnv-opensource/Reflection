#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CallableArguments.h"
#include "CallableArgumentsVararg.h"
#include "TypeUtilities/FunctionTraits.h"
#include "Modifier.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    template<int Arity,bool IsVarargValue>
    class CallableArgumentsSelector : public CallableArguments
    {
    public:
        CallableArgumentsSelector(const std::vector<Variants::Variant>& args,MemberPointer member)
            : CallableArguments(args,member) {}
    };
    template<int Arity>
    class CallableArgumentsSelector<Arity,true> : public CallableArgumentsVararg<Arity-1>
    {
    public:
        CallableArgumentsSelector(const std::vector<Variants::Variant>& args,MemberPointer member)
            : CallableArgumentsVararg<Arity-1>(args,member) {}
    };
}}}}