//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsPressure.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Pressure.h>
#include "jsStress.h"
#include "jsForcePerUnitArea.h"
#include "jsTranslationalStiffnessPerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsPressure::GetPhenomenon() 
{ 
    return PressurePhenomenon();
}

void jsPressure::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsPressure,DNVS::MoFa::Units::Pressure> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsPressure,DNVS::MoFa::Units::Pressure>::ConstructEquivalentQuantity<jsForcePerUnitArea>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsPressure,DNVS::MoFa::Units::Pressure>::ConstructEquivalentQuantity<jsStress>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsPressure,DNVS::MoFa::Units::Pressure>::ConstructEquivalentQuantity<jsTranslationalStiffnessPerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsPressure& val) {return ForcePerUnitArea(val); });
   cls.ImplicitConstructorConversion([](const jsPressure& val) {return Stress(val); });
   cls.ImplicitConstructorConversion([](const jsPressure& val) {return TranslationalStiffnessPerUnitLength(val); });
}


