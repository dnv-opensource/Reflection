//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalStiffnessPerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalStiffnessPerUnitLength.h>
#include "jsAxialStiffness.h"
#include "jsCoupledStiffness.h"
#include "jsForce.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalStiffnessPerUnitLength::GetPhenomenon() 
{ 
    return RotationalStiffnessPerUnitLengthPhenomenon();
}

void jsRotationalStiffnessPerUnitLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalStiffnessPerUnitLength,DNVS::MoFa::Units::RotationalStiffnessPerUnitLength> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalStiffnessPerUnitLength,DNVS::MoFa::Units::RotationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsForce>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalStiffnessPerUnitLength,DNVS::MoFa::Units::RotationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsCoupledStiffness>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalStiffnessPerUnitLength,DNVS::MoFa::Units::RotationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsAxialStiffness>);
   cls.ImplicitConstructorConversion([](const jsRotationalStiffnessPerUnitLength& val) {return Force(val); });
   cls.ImplicitConstructorConversion([](const jsRotationalStiffnessPerUnitLength& val) {return CoupledStiffness(val); });
   cls.ImplicitConstructorConversion([](const jsRotationalStiffnessPerUnitLength& val) {return AxialStiffness(val); });
}


