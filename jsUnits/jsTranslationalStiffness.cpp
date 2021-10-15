//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTranslationalStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TranslationalStiffness.h>
#include "jsForcePerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTranslationalStiffness::GetPhenomenon() 
{ 
    return TranslationalStiffnessPhenomenon();
}

void jsTranslationalStiffness::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTranslationalStiffness,DNVS::MoFa::Units::TranslationalStiffness> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsTranslationalStiffness,DNVS::MoFa::Units::TranslationalStiffness>::ConstructEquivalentQuantity<jsForcePerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsTranslationalStiffness& stiffness) {return ForcePerUnitLength(stiffness); });
}


