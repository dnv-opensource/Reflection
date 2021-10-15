//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsArea.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Area.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsArea::GetPhenomenon()
{
    return AreaPhenomenon();
}

void jsArea::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsArea,Area> cls(typeLibrary);
}


