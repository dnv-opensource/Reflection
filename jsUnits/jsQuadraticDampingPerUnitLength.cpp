//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsQuadraticDampingPerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/QuadraticDampingPerUnitLength.h>
#include "jsMassPerUnitArea.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsQuadraticDampingPerUnitLength::GetPhenomenon() 
{ 
    return QuadraticDampingPerUnitLengthPhenomenon();
}

void jsQuadraticDampingPerUnitLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsQuadraticDampingPerUnitLength,DNVS::MoFa::Units::QuadraticDampingPerUnitLength> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsQuadraticDampingPerUnitLength,DNVS::MoFa::Units::QuadraticDampingPerUnitLength>::ConstructEquivalentQuantity<jsMassPerUnitArea>);
   cls.ImplicitConstructorConversion([](const jsQuadraticDampingPerUnitLength& val) {return MassPerUnitArea(val); });
}


