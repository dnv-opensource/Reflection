//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsEnergyDensity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/EnergyDensity.h>
#include "jsDisplacementResponseSpectrum.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsEnergyDensity::GetPhenomenon()
{
    return EnergyDensityPhenomenon();
}

void jsEnergyDensity::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsEnergyDensity, EnergyDensity> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsEnergyDensity, EnergyDensity>::ConstructEquivalentQuantity<jsDisplacementResponseSpectrum>);
    cls.ImplicitConstructorConversion([](const jsEnergyDensity& val) {return DisplacementResponseSpectrum(val); });
}


