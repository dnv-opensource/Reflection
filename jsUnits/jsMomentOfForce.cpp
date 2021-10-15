//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMomentOfForce.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MomentOfForce.h>
#include "jsEnergy.h"
#include "jsRotationalStiffness.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMomentOfForce::GetPhenomenon() 
{ 
    return MomentOfForcePhenomenon();
}

void jsMomentOfForce::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMomentOfForce,DNVS::MoFa::Units::MomentOfForce> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMomentOfForce,DNVS::MoFa::Units::MomentOfForce>::ConstructEquivalentQuantity<jsEnergy>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMomentOfForce,DNVS::MoFa::Units::MomentOfForce>::ConstructEquivalentQuantity<jsRotationalStiffness>);      
   cls.ImplicitConstructorConversion([](const jsMomentOfForce& val) {return Energy(val); });
   cls.ImplicitConstructorConversion([](const jsMomentOfForce& val) {return RotationalStiffness(val); });
}


