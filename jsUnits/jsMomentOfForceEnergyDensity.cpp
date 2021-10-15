//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMomentOfForceEnergyDensity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MomentOfForceEnergyDensity.h>
#include "jsMomentResponseSpectrum.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMomentOfForceEnergyDensity::GetPhenomenon() 
{ 
    return MomentOfForceEnergyDensityPhenomenon();
}

void jsMomentOfForceEnergyDensity::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMomentOfForceEnergyDensity,DNVS::MoFa::Units::MomentOfForceEnergyDensity> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMomentOfForceEnergyDensity,DNVS::MoFa::Units::MomentOfForceEnergyDensity>::ConstructEquivalentQuantity<jsMomentResponseSpectrum>);
   cls.ImplicitConstructorConversion([](const jsMomentOfForceEnergyDensity& val) {return MomentResponseSpectrum(val); });
}


