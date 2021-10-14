#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Operators/Stringizer.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Members {
    template<typename Tag>
    struct BinaryOperatorName {
        static std::string Name() {return "__" + DNVS::MoFa::Operators::Stringizer<Tag>::Name() + "__";}
    };
    template<typename Tag>
    struct BinaryOperatorRName {
        static std::string Name() {return "__" + DNVS::MoFa::Operators::Stringizer<Tag>::Name() + "R__";}
    };
    template<typename Tag>
    struct UnaryOperatorName {
        static std::string Name() {return "__" + DNVS::MoFa::Operators::Stringizer<Tag>::Name() + "__";}
    };
}}}}