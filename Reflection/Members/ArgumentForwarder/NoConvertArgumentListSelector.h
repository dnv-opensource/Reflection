#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "NoConvertArgumentList.h"
#include "NoConvertArgumentListVararg.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members { namespace ArgumentForwarder {

    template<int Arity, bool IsVarargValue>
    class NoConvertArgumentListSelector : public NoConvertArgumentList
    {
    public:
        NoConvertArgumentListSelector(const std::vector<Variants::Variant>& args)
            : NoConvertArgumentList(args) {}
    };
    template<int Arity>
    class NoConvertArgumentListSelector<Arity, true> : public NoConvertArgumentListVararg<Arity - 1>
    {
    public:
        NoConvertArgumentListSelector(const std::vector<Variants::Variant>& args)
            : NoConvertArgumentListVararg<Arity - 1>(args) {}
    };
}}}}}