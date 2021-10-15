//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsTemperatureGradient.h"
#include <jsUnits/jsUnitClass.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTemperatureGradient::GetPhenomenon()
{
    return TempGradientPhenomenon();
}

void jsTemperatureGradient::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsTemperatureGradient, DNVS::MoFa::Units::TempGradient> cls(typeLibrary);
    if (cls.reinit()) return;
}


