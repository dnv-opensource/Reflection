//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTranslationalStiffnessPerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TranslationalStiffnessPerUnitLength.h>
#include "jsStress.h"
#include "jsPressure.h"
#include "jsForcePerUnitArea.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTranslationalStiffnessPerUnitLength::GetPhenomenon() 
{ 
    return TranslationalStiffnessPerUnitLengthPhenomenon();
}

void jsTranslationalStiffnessPerUnitLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTranslationalStiffnessPerUnitLength,DNVS::MoFa::Units::TranslationalStiffnessPerUnitLength> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsTranslationalStiffnessPerUnitLength,DNVS::MoFa::Units::TranslationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsForcePerUnitArea>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsTranslationalStiffnessPerUnitLength,DNVS::MoFa::Units::TranslationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsPressure>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsTranslationalStiffnessPerUnitLength,DNVS::MoFa::Units::TranslationalStiffnessPerUnitLength>::ConstructEquivalentQuantity<jsStress>);
   cls.ImplicitConstructorConversion([](const jsTranslationalStiffnessPerUnitLength& val) {return ForcePerUnitArea(val); });
   cls.ImplicitConstructorConversion([](const jsTranslationalStiffnessPerUnitLength& val) {return Pressure(val); });
   cls.ImplicitConstructorConversion([](const jsTranslationalStiffnessPerUnitLength& val) {return Stress(val); });
}
