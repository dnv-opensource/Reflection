//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTranslationalQuadraticDamping.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TranslationalQuadraticDamping.h>
#include "jsMassPerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTranslationalQuadraticDamping::GetPhenomenon() 
{ 
    return TranslationalQuadraticDampingPhenomenon();
}

void jsTranslationalQuadraticDamping::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTranslationalQuadraticDamping,DNVS::MoFa::Units::TranslationalQuadraticDamping> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsTranslationalQuadraticDamping,DNVS::MoFa::Units::TranslationalQuadraticDamping>::ConstructEquivalentQuantity<jsMassPerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsTranslationalQuadraticDamping& val) {return MassPerUnitLength(val); });
}


