//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsAxialStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/AxialStiffness.h>
#include "jsRotationalStiffnessPerUnitLength.h"
#include "jsCoupledStiffness.h"
#include "jsForce.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsAxialStiffness::GetPhenomenon()
{
    return AxialStiffnessPhenomenon();
}

void jsAxialStiffness::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsAxialStiffness, AxialStiffness> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsAxialStiffness, AxialStiffness>::ConstructEquivalentQuantity<jsRotationalStiffnessPerUnitLength>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsAxialStiffness, AxialStiffness>::ConstructEquivalentQuantity<jsCoupledStiffness>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsAxialStiffness, AxialStiffness>::ConstructEquivalentQuantity<jsForce>);
    cls.ImplicitConstructorConversion([](const jsAxialStiffness& val) {return RotationalStiffnessPerUnitLength(val); });
    cls.ImplicitConstructorConversion([](const jsAxialStiffness& val) {return CoupledStiffness(val); });
    cls.ImplicitConstructorConversion([](const jsAxialStiffness& val) {return Force(val); });

}


