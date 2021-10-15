//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsLength.h>
#include <jsUnits/jsUnitClass.h>
#include "Units/Length.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsLength::GetPhenomenon() 
{ 
    return LengthPhenomenon();
}


void jsLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsLength,DNVS::MoFa::Units::Length> cls(typeLibrary);
   if(cls.reinit()) return;

   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer4 = "&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Length");
   example.AddScript("<b>Length units</b>" + spacer18 + "<b>Conversion factor</b>");
   example.AddScript("m(SI base unit)"+ spacer4 + spacer20 + "= " + std::string( mofa::lexical_cast<std::string>(_m.GetValue() )));
   example.AddScript("cm" + spacer20 + spacer20 + "&nbsp;&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_cm.GetValue() )));
   example.AddScript("mm" + spacer20 + spacer20 + "&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_mm.GetValue() )));
   example.AddScript("ft " + spacer20 + spacer20 + "&nbsp;&nbsp;&nbsp;&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_ft.GetValue() )));
   example.AddScript("in " + spacer20 + spacer20 + "&nbsp;&nbsp;&nbsp;= " + std::string( mofa::lexical_cast<std::string>(_in.GetValue() )));
}
