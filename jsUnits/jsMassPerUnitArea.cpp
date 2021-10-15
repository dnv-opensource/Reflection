//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMassPerUnitArea.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MassPerUnitArea.h>
#include "jsQuadraticDampingPerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMassPerUnitArea::GetPhenomenon() 
{ 
    return MassPerUnitAreaPhenomenon();
}

void jsMassPerUnitArea::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMassPerUnitArea,DNVS::MoFa::Units::MassPerUnitArea> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMassPerUnitArea,DNVS::MoFa::Units::MassPerUnitArea>::ConstructEquivalentQuantity<jsQuadraticDampingPerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsMassPerUnitArea& val) {return QuadraticDampingPerUnitLength(val); });
}


