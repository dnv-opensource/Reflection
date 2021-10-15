//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsStress.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Stress.h>
#include "jsForcePerUnitArea.h"
#include "jsPressure.h"
#include "jsTranslationalStiffnessPerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsStress::GetPhenomenon() 
{ 
    return StressPhenomenon();
}

void jsStress::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsStress,DNVS::MoFa::Units::Stress> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsStress,DNVS::MoFa::Units::Stress>::ConstructEquivalentQuantity<jsForcePerUnitArea>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsStress,DNVS::MoFa::Units::Stress>::ConstructEquivalentQuantity<jsPressure>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsStress,DNVS::MoFa::Units::Stress>::ConstructEquivalentQuantity<jsTranslationalStiffnessPerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsStress& val) {return ForcePerUnitArea(val); });
   cls.ImplicitConstructorConversion([](const jsStress& val) {return Pressure(val); });
   cls.ImplicitConstructorConversion([](const jsStress& val) {return TranslationalStiffnessPerUnitLength(val); });
}


