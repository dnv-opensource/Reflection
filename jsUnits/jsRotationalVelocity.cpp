//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalVelocity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalVelocity.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalVelocity::GetPhenomenon() 
{ 
    return RotationalVelocityPhenomenon();
}

void jsRotationalVelocity::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalVelocity,DNVS::MoFa::Units::RotationalVelocity> cls(typeLibrary);
}


