//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationPerLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationPerLength.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationPerLength::GetPhenomenon() 
{ 
    return RotationPerLengthPhenomenon();
}

void jsRotationPerLength::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationPerLength,DNVS::MoFa::Units::RotationPerLength> cls(typeLibrary);
}
