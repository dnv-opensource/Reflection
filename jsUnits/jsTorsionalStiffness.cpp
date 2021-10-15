//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTorsionalStiffness.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TorsionalStiffness.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTorsionalStiffness::GetPhenomenon() 
{ 
    return TorsionalStiffnessPhenomenon();
}

void jsTorsionalStiffness::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTorsionalStiffness,DNVS::MoFa::Units::TorsionalStiffness> cls(typeLibrary);
}


