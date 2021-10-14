#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Classes {
    template<typename Unit, typename ReflectionClassT, typename ClassT, typename Arg1, typename Arg2>
    auto& SetGetWithUnit(ReflectionClassT& cls, const std::string& name, void (ClassT::*setter)(Arg1), Arg2 (ClassT::*getter)() const)
    {
        auto lambdaWithSetter = [=](ClassT & section, Unit unit) {(section.*setter)(unit.GetValue()); };
        auto lambdaWithGetter = [=](const ClassT & section) {return Unit((section.*getter)()); };
        return cls.SetGet(name, lambdaWithSetter, lambdaWithGetter);
    }
}}}}
