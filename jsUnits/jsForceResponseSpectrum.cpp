//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForceResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForceResponseSpectrum.h>
#include "jsForceEnergyDensity.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForceResponseSpectrum::GetPhenomenon() 
{ 
    return ForceResponseSpectrumPhenomenon();
}

void jsForceResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsForceResponseSpectrum,DNVS::MoFa::Units::ForceResponseSpectrum> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsForceResponseSpectrum,DNVS::MoFa::Units::ForceResponseSpectrum>::ConstructEquivalentQuantity<jsForceEnergyDensity>);
   cls.ImplicitConstructorConversion([](const jsForceResponseSpectrum& val) {return ForceEnergyDensity(val); });
}


