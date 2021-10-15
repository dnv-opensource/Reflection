//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsThermalExpansionCoeff.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/ThermalExpansionCoeff.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsThermalExpansionCoeff::GetPhenomenon() 
{ 
    return ThermalExpansionCoeffPhenomenon();
}

void jsThermalExpansionCoeff::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsThermalExpansionCoeff,DNVS::MoFa::Units::ThermalExpansionCoeff> cls(typeLibrary);
}


