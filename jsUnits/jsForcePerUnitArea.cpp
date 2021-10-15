//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForcePerUnitArea.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForcePerUnitArea.h>
#include "jsStress.h"
#include "jsPressure.h"
#include "jsTranslationalStiffnessPerUnitLength.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForcePerUnitArea::GetPhenomenon() 
{ 
    return ForcePerUnitAreaPhenomenon();
}

void jsForcePerUnitArea::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsForcePerUnitArea,DNVS::MoFa::Units::ForcePerUnitArea> cls(typeLibrary);

   if(cls.reinit()) return;

   cls.ImplicitConstructorConversion(&jsUnitClass<jsForcePerUnitArea,DNVS::MoFa::Units::ForcePerUnitArea>::ConstructEquivalentQuantity<jsStress>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsForcePerUnitArea,DNVS::MoFa::Units::ForcePerUnitArea>::ConstructEquivalentQuantity<jsPressure>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsForcePerUnitArea,DNVS::MoFa::Units::ForcePerUnitArea>::ConstructEquivalentQuantity<jsTranslationalStiffnessPerUnitLength>);
   cls.ImplicitConstructorConversion([](const jsForcePerUnitArea& val) {return Stress(val); });
   cls.ImplicitConstructorConversion([](const jsForcePerUnitArea& val) {return Pressure(val); });
   cls.ImplicitConstructorConversion([](const jsForcePerUnitArea& val) {return TranslationalStiffnessPerUnitLength(val); });
   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("ForcePerUnitArea");
   example.AddScript("<b>ForcePerUnitArea</b> &nbsp;&nbsp;" + spacer10 + "<b>Conversion factor</b>");
   example.AddScript("Pa (SI base unit)&nbsp;" + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_Pa.GetValue() )));
   example.AddScript("psi" + spacer20 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_psi.GetValue() )));
   example.AddScript("ksi" + spacer20 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_ksi.GetValue() )));
   example.AddScript("atm&nbsp;" + spacer20 + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_atm.GetValue() )));
   example.AddScript("bar" + spacer20 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_bar.GetValue() )));
}
