//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsDampingPerUnitLength.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/DampingPerUnitLength.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsDampingPerUnitLength::GetPhenomenon()
{
    return DampingPerUnitLengthPhenomenon();
}

void jsDampingPerUnitLength::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsDampingPerUnitLength, DampingPerUnitLength> cls(typeLibrary);
}


