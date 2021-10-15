//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalFlexibility.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalFlexibility.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalFlexibility::GetPhenomenon() 
{ 
    return RotationalFlexibilityPhenomenon();
}

void jsRotationalFlexibility::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalFlexibility,DNVS::MoFa::Units::RotationalFlexibility> cls(typeLibrary);
}


