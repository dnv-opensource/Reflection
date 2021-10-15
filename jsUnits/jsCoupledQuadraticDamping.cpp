//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledQuadraticDamping.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledQuadraticDamping.h>
#include "jsCoupledQuadraticDampingMoment.h"
#include "jsMass.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledQuadraticDamping::GetPhenomenon()
{
    return CoupledQuadraticDampingPhenomenon();
}

void jsCoupledQuadraticDamping::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsCoupledQuadraticDamping,CoupledQuadraticDamping> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledQuadraticDamping,CoupledQuadraticDamping>::ConstructEquivalentQuantity<jsCoupledQuadraticDampingMoment>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledQuadraticDamping,CoupledQuadraticDamping>::ConstructEquivalentQuantity<jsMass>);
   cls.ImplicitConstructorConversion([](const jsCoupledQuadraticDamping& val) {return CoupledQuadraticDampingMoment(val); });
   cls.ImplicitConstructorConversion([](const jsCoupledQuadraticDamping& val) {return Mass(val); });
}


