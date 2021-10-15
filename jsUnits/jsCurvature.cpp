//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCurvature.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Curvature.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCurvature::GetPhenomenon()
{
    return CurvaturePhenomenon();
}

void jsCurvature::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsCurvature,Curvature> cls(typeLibrary);
}
