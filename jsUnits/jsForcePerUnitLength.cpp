//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForcePerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForcePerUnitLength.h>
#include "jsTranslationalStiffness.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForcePerUnitLength::GetPhenomenon() 
{ 
    return ForcePerUnitLengthPhenomenon();
}

void jsForcePerUnitLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsForcePerUnitLength,DNVS::MoFa::Units::ForcePerUnitLength> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsForcePerUnitLength,DNVS::MoFa::Units::ForcePerUnitLength>::ConstructEquivalentQuantity<jsTranslationalStiffness>);
   cls.ImplicitConstructorConversion([](const jsForcePerUnitLength& val) {return TranslationalStiffness(val); });
}


