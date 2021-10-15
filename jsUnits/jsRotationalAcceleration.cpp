//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalAcceleration.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalAcceleration.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalAcceleration::GetPhenomenon() 
{ 
    return RotationalAccelerationPhenomenon();
}

void jsRotationalAcceleration::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalAcceleration,DNVS::MoFa::Units::RotationalAcceleration> cls(typeLibrary);
}


