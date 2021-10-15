//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledQuadraticDampingMoment.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledQuadraticDampingMoment.h>
#include "jsMass.h"
#include "jsCoupledQuadraticDamping.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledQuadraticDampingMoment::GetPhenomenon()
{
    return CoupledQuadraticDampingMomentPhenomenon();
}

void jsCoupledQuadraticDampingMoment::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsCoupledQuadraticDampingMoment, CoupledQuadraticDampingMoment> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledQuadraticDampingMoment, CoupledQuadraticDampingMoment>::ConstructEquivalentQuantity<jsMass>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledQuadraticDampingMoment, CoupledQuadraticDampingMoment>::ConstructEquivalentQuantity<jsCoupledQuadraticDamping>);
    cls.ImplicitConstructorConversion([](const jsCoupledQuadraticDampingMoment& val) {return Mass(val); });
    cls.ImplicitConstructorConversion([](const jsCoupledQuadraticDampingMoment& val) {return CoupledQuadraticDamping(val); });
}


