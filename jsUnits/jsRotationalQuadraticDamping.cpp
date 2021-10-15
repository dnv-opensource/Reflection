//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalQuadraticDamping.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalQuadraticDamping.h>
#include "jsMassMomentOfInertia.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalQuadraticDamping::GetPhenomenon() 
{ 
    return RotationalQuadraticDampingPhenomenon();
}

void jsRotationalQuadraticDamping::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalQuadraticDamping,DNVS::MoFa::Units::RotationalQuadraticDamping> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsRotationalQuadraticDamping,DNVS::MoFa::Units::RotationalQuadraticDamping>::ConstructEquivalentQuantity<jsMassMomentOfInertia>);
   cls.ImplicitConstructorConversion([](const jsRotationalQuadraticDamping& val) {return MassMomentOfInertia(val); });
}


