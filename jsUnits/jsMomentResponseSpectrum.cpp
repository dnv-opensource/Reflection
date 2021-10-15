//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMomentResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MomentResponseSpectrum.h>
#include "jsMomentOfForceEnergyDensity.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMomentResponseSpectrum::GetPhenomenon() 
{ 
    return MomentResponseSpectrumPhenomenon();
}

void jsMomentResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMomentResponseSpectrum,DNVS::MoFa::Units::MomentResponseSpectrum> cls(typeLibrary);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMomentResponseSpectrum,DNVS::MoFa::Units::MomentResponseSpectrum>::ConstructEquivalentQuantity<jsMomentOfForceEnergyDensity>);
   cls.ImplicitConstructorConversion([](const jsMomentResponseSpectrum& val) {return MomentOfForceEnergyDensity(val); });
}


