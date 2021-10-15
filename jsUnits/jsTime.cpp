//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTime.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Time.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTime::GetPhenomenon() 
{ 
    return TimePhenomenon();
}

void jsTime::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTime,DNVS::MoFa::Units::Time> cls(typeLibrary);

   if (cls.reinit())
       return;

   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Time");
   example.AddScript("<b>Time units</b> &nbsp;&nbsp;" + spacer14 + "<b>Conversion factor</b>");
   example.AddScript("s (SI base unit) &nbsp;&nbsp;&nbsp;" + spacer14 + "= " + std::string( mofa::lexical_cast<std::string>(_s.GetValue() )));
   example.AddScript("min " + spacer20 + spacer14 + "= " + std::string( mofa::lexical_cast<std::string>(_min.GetValue() )));
   example.AddScript("hr" + spacer20 + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_hr.GetValue() )));
}
