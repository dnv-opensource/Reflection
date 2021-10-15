//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForcePerUnitAreaPerAngle.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForcePerUnitAreaPerAngle.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForcePerUnitAreaPerAngle::GetPhenomenon() 
{ 
    return ForcePerUnitAreaPerAnglePhenomenon();
}

void jsForcePerUnitAreaPerAngle::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsForcePerUnitAreaPerAngle,DNVS::MoFa::Units::ForcePerUnitAreaPerAngle> cls(typeLibrary);
}


