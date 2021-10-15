//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMass.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Mass.h>
#include "jsCoupledQuadraticDampingMoment.h"
#include "jsCoupledQuadraticDamping.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMass::GetPhenomenon()
{
    return MassPhenomenon();
}

void jsMass::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMass,DNVS::MoFa::Units::Mass> cls(typeLibrary);

   if (cls.reinit())
       return;

   cls.ImplicitConstructorConversion(&jsUnitClass<jsMass,DNVS::MoFa::Units::Mass>::ConstructEquivalentQuantity<jsCoupledQuadraticDampingMoment>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsMass,DNVS::MoFa::Units::Mass>::ConstructEquivalentQuantity<jsCoupledQuadraticDamping>);
   cls.ImplicitConstructorConversion([](const jsMass& val) {return CoupledQuadraticDampingMoment(val); });
   cls.ImplicitConstructorConversion([](const jsMass& val) {return CoupledQuadraticDamping(val); });
   std::string spacer6 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer12 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer16 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Mass");
   example.AddScript("<b>Mass units</b>" + spacer18 + "<b>Conversion factor</b>");
   example.AddScript("kg (SI base unit)"+ spacer18 + "&nbsp;&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_kg.GetValue())));
   example.AddScript("tonne (metric ton)" + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_tonne.GetValue())));
   example.AddScript("lb" + spacer18 + spacer18 + spacer6 + "= " + std::string( mofa::lexical_cast<std::string>(_lb.GetValue())));
   example.AddScript("kips" + spacer18 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_kips.GetValue())));
   example.AddScript("slug" + spacer18 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_slug.GetValue())));
   example.AddScript("ton (UK long ton)" + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_ton.GetValue())));
}
