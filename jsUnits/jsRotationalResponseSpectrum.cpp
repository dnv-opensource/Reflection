//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsRotationalResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/RotationalResponseSpectrum.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsRotationalResponseSpectrum::GetPhenomenon() 
{ 
    return RotationalResponseSpectrumPhenomenon();
}

void jsRotationalResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsRotationalResponseSpectrum,DNVS::MoFa::Units::RotationalResponseSpectrum> cls(typeLibrary);
}


