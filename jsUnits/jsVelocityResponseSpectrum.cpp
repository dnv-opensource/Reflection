//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsVelocityResponseSpectrum.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/VelocityResponseSpectrum.h>
#include "jsKinematicViscosity.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsVelocityResponseSpectrum::GetPhenomenon() 
{ 
    return VelocityResponseSpectrumPhenomenon();
}

void jsVelocityResponseSpectrum::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsVelocityResponseSpectrum,DNVS::MoFa::Units::VelocityResponseSpectrum> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsVelocityResponseSpectrum,DNVS::MoFa::Units::VelocityResponseSpectrum>::ConstructEquivalentQuantity<jsKinematicViscosity>);
   cls.ImplicitConstructorConversion([](const jsVelocityResponseSpectrum& val) {return KinematicViscosity(val); });
}


