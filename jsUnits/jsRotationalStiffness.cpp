//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalStiffness.h>
#include "jsMomentOfForce.h"
#include "jsEnergy.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalStiffness::GetPhenomenon() 
{ 
    return RotationalStiffnessPhenomenon();
}

void jsRotationalStiffness::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalStiffness,DNVS::MoFa::Units::RotationalStiffness> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalStiffness,DNVS::MoFa::Units::RotationalStiffness>::ConstructEquivalentQuantity<jsMomentOfForce>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalStiffness,DNVS::MoFa::Units::RotationalStiffness>::ConstructEquivalentQuantity<jsEnergy>);      
   cls.ImplicitConstructorConversion([](const jsRotationalStiffness& val) {return MomentOfForce(val); });
   cls.ImplicitConstructorConversion([](const jsRotationalStiffness& val) {return Energy(val); });
}


