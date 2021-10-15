//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledStiffness.h>
#include "jsRotationalStiffnessPerUnitLength.h"
#include "jsForce.h"
#include "jsAxialStiffness.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledStiffness::GetPhenomenon()
{
    return CoupledStiffnessPhenomenon();
}

void jsCoupledStiffness::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsCoupledStiffness,CoupledStiffness> cls(typeLibrary);
    if(cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledStiffness,CoupledStiffness>::ConstructEquivalentQuantity<jsRotationalStiffnessPerUnitLength>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledStiffness,CoupledStiffness>::ConstructEquivalentQuantity<jsForce>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledStiffness,CoupledStiffness>::ConstructEquivalentQuantity<jsAxialStiffness>);
    cls.ImplicitConstructorConversion([](const jsCoupledStiffness& val) {return RotationalStiffnessPerUnitLength(val); });
    cls.ImplicitConstructorConversion([](const jsCoupledStiffness& val) {return Force(val); });
    cls.ImplicitConstructorConversion([](const jsCoupledStiffness& val) {return AxialStiffness(val); });
}


