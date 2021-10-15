//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForcePerUnitVolume.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ForcePerUnitVolume.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForcePerUnitVolume::GetPhenomenon() 
{ 
    return ForcePerUnitVolumePhenomenon();
}

void jsForcePerUnitVolume::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsForcePerUnitVolume,DNVS::MoFa::Units::ForcePerUnitVolume> cls(typeLibrary);
}


