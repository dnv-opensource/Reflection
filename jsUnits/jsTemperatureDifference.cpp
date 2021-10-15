//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTemperatureDifference.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TempDiff.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTemperatureDifference::GetPhenomenon() 
{ 
    return TempDiffPhenomenon();
}

void jsTemperatureDifference::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTemperatureDifference,DNVS::MoFa::Units::TempDiff> cls(typeLibrary);
   if(cls.reinit()) return;

   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Acceleration");
   example.AddScript("<b>TempDiff units</b> &nbsp;&nbsp;" + spacer10 + "<b>Conversion factor</b>");
   example.AddScript("delC (Centigrade)" + spacer14 + "= " + std::string( mofa::lexical_cast<std::string>(_delC.GetValue() )));
   example.AddScript("delF (Fahrenheit) " + spacer14 + "= " + std::string( mofa::lexical_cast<std::string>(_delF.GetValue() )));
}
