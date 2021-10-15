//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsMassDensity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/MassDensity.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsMassDensity::GetPhenomenon() 
{ 
    return MassDensityPhenomenon();
}

void jsMassDensity::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsMassDensity,DNVS::MoFa::Units::MassDensity> cls(typeLibrary);
}


