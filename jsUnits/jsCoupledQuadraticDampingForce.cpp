//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledQuadraticDampingForce.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledQuadraticDampingForce.h>
#include "jsCoupledMass.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledQuadraticDampingForce::GetPhenomenon()
{
    return CoupledQuadraticDampingForcePhenomenon();
}

void jsCoupledQuadraticDampingForce::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsCoupledQuadraticDampingForce, CoupledQuadraticDampingForce> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledQuadraticDampingForce, CoupledQuadraticDampingForce>::ConstructEquivalentQuantity<jsCoupledMass>);
    cls.ImplicitConstructorConversion([](const jsCoupledQuadraticDampingForce& val) {return CoupledMass(val); });
}


