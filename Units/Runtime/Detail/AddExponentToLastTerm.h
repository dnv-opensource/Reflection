#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnit.h"
#include "PowerUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {
    UnitPointer AddExponentToLastTerm(UnitPointer unit, int exponent)
    {
        std::list<UnitPointer> children = unit->GetChildren();
        if(!children.empty())
        {
            children.back() = AddExponentToLastTerm(children.back(), exponent);
            return Detail::UnitPointer(unit->CreateCopy(children));
        }
        else
        {
            return PowerUnit::CreatePowerUnit(unit, exponent);
        }
    }
}}}}}