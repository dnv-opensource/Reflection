//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCoupledDamping.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CoupledDamping.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCoupledDamping::GetPhenomenon()
{
    return CoupledDampingPhenomenon();
}

void jsCoupledDamping::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsCoupledDamping, CoupledDamping> cls(typeLibrary);
}


