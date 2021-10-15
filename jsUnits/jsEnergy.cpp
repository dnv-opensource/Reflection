//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsEnergy.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Energy.h>
#include "jsMomentOfForce.h"
#include "jsRotationalStiffness.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsEnergy::GetPhenomenon()
{
    return EnergyPhenomenon();
}

void jsEnergy::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsEnergy,Energy> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsEnergy,Energy>::ConstructEquivalentQuantity<jsMomentOfForce>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsEnergy,Energy>::ConstructEquivalentQuantity<jsRotationalStiffness>);      
   cls.ImplicitConstructorConversion([](const jsEnergy& val) {return MomentOfForce(val); });
   cls.ImplicitConstructorConversion([](const jsEnergy& val) {return RotationalStiffness(val); });
}


