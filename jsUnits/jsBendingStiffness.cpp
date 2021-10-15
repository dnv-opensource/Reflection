//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsBendingStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/BendingStiffness.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsBendingStiffness::GetPhenomenon()
{
    return BendingStiffnessPhenomenon();
}

void jsBendingStiffness::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsBendingStiffness, BendingStiffness> cls(typeLibrary);
}


