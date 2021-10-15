//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMassMomentOfInertia.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MassMomentOfInertia.h>
#include "jsRotationalQuadraticDamping.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMassMomentOfInertia::GetPhenomenon() 
{ 
    return MassMomentOfInertiaPhenomenon();
}

void jsMassMomentOfInertia::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMassMomentOfInertia,DNVS::MoFa::Units::MassMomentOfInertia> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMassMomentOfInertia,DNVS::MoFa::Units::MassMomentOfInertia>::ConstructEquivalentQuantity<jsRotationalQuadraticDamping>);
   cls.ImplicitConstructorConversion([](const jsMassMomentOfInertia& val) {return RotationalQuadraticDamping(val); });
}


