//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMassPerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MassPerUnitLength.h>
#include "jsTranslationalQuadraticDamping.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMassPerUnitLength::GetPhenomenon() 
{ 
    return MassPerUnitLengthPhenomenon();
}

void jsMassPerUnitLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMassPerUnitLength,DNVS::MoFa::Units::MassPerUnitLength> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMassPerUnitLength,DNVS::MoFa::Units::MassPerUnitLength>::ConstructEquivalentQuantity<jsTranslationalQuadraticDamping>);
   cls.ImplicitConstructorConversion([](const jsMassPerUnitLength& val) {return TranslationalQuadraticDamping(val); });
}


