//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsCurvatureResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/CurvatureResponseSpectrum.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsCurvatureResponseSpectrum::GetPhenomenon()
{
    return CurvatureResponseSpectrumPhenomenon();
}

void jsCurvatureResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsCurvatureResponseSpectrum, CurvatureResponseSpectrum> cls(typeLibrary);
}


