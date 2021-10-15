//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsSpecificHeat.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/SpecificHeat.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsSpecificHeat::GetPhenomenon() 
{ 
    return SpecificHeatPhenomenon();
}

void jsSpecificHeat::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsSpecificHeat,DNVS::MoFa::Units::SpecificHeat> cls(typeLibrary);
}


