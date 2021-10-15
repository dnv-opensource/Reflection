//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <jsUnits/jsAngle.h>
#include <jsUnits/jsUnitClass.h>
#include "Units/Angle.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsAngle::GetPhenomenon()
{
    return AnglePhenomenon();
}

void jsAngle::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsAngle,Angle> cls(typeLibrary);

   if (cls.reinit())
       return;

   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer16 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Angle");
   example.AddScript("<b>Angle units</b>&nbsp;" + spacer14 + "<b>Conversion factor</b>");
   example.AddScript("rad (SI base unit)" + spacer14 + "&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_rad.GetValue() )));
   example.AddScript("deg (degree) &nbsp;&nbsp;"  + spacer18 + "&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_deg.GetValue() )));
   example.AddScript("rev (revolution)&nbsp;&nbsp;" + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_rev.GetValue())));
}
