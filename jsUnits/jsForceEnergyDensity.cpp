//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForceEnergyDensity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForceEnergyDensity.h>
#include "jsForceResponseSpectrum.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForceEnergyDensity::GetPhenomenon() 
{ 
    return ForceEnergyDensityPhenomenon();
}

void jsForceEnergyDensity::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsForceEnergyDensity, DNVS::MoFa::Units::ForceEnergyDensity> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsForceEnergyDensity, DNVS::MoFa::Units::ForceEnergyDensity>::ConstructEquivalentQuantity<jsForceResponseSpectrum>);
    cls.ImplicitConstructorConversion([](const jsForceEnergyDensity& val) {return ForceResponseSpectrum(val); });
}


