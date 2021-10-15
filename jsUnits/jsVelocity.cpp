//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsVelocity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Velocity.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsVelocity::GetPhenomenon() 
{ 
    return VelocityPhenomenon();
}

void jsVelocity::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsVelocity,DNVS::MoFa::Units::Velocity> cls(typeLibrary);

   if (cls.reinit())
       return;

   std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
   std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

   auto& example = cls.AddExample("Velocity");
   example.AddScript("<b>Velocity units</b> &nbsp;&nbsp;" + spacer10 + "<b>Conversion factor</b>");
   example.AddScript("m/s (SI derived unit) &nbsp;" + spacer14 + "= " + std::string( mofa::lexical_cast<std::string>((_m/_s).GetValue() )));
   example.AddScript("Knot&nbsp;&nbsp;" + spacer18 + spacer18 + "= " + std::string( mofa::lexical_cast<std::string>(_Knot.GetValue() )));
}
