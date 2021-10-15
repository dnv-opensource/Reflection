//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsDisplacementResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/DisplacementResponseSpectrum.h>
#include "jsEnergyDensity.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsDisplacementResponseSpectrum::GetPhenomenon()
{
    return DisplacementResponseSpectrumPhenomenon();
}


void jsDisplacementResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsDisplacementResponseSpectrum,DisplacementResponseSpectrum> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsDisplacementResponseSpectrum,DisplacementResponseSpectrum>::ConstructEquivalentQuantity<jsEnergyDensity>);
   cls.ImplicitConstructorConversion([](const jsDisplacementResponseSpectrum& val) {return EnergyDensity(val); });
}


