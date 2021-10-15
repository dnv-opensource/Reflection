//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <jsUnits/jsAcceleration.h>
#include <jsUnits/jsUnitClass.h>
#include "Units/Acceleration.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsAcceleration::GetPhenomenon()
{
    return AccelerationPhenomenon();
}

void jsAcceleration::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsAcceleration,Acceleration> cls(typeLibrary);
   if(cls.reinit()) return;

   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer12 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Acceleration");
   example.AddScript("<b>Acceleration units</b> &nbsp;&nbsp;" + spacer10 + "<b>Conversion factor</b>");
   example.AddScript("m/s^2 (SI derived unit)" + spacer12 + "&nbsp;&nbsp;= " + std::string( mofa::lexical_cast<std::string>((_m/_s2).GetValue())));
   example.AddScript("m/s^2 (G (gravity) " + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_G.GetValue())));
}
