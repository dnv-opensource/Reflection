//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledMass.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledMass.h>
#include "jsCoupledQuadraticDampingForce.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledMass::GetPhenomenon()
{
    return CoupledMassPhenomenon();
}

void jsCoupledMass::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsCoupledMass,CoupledMass> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsCoupledMass,CoupledMass>::ConstructEquivalentQuantity<jsCoupledQuadraticDampingForce>);
   cls.ImplicitConstructorConversion([](const jsCoupledMass& val) {return CoupledQuadraticDampingForce(val); });
}


