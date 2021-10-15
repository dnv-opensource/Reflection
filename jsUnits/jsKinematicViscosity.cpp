//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsKinematicViscosity.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/KinematicViscosity.h>
#include "jsVelocityResponseSpectrum.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsKinematicViscosity::GetPhenomenon() 
{ 
    return KinematicViscosityPhenomenon();
}

void jsKinematicViscosity::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsKinematicViscosity,DNVS::MoFa::Units::KinematicViscosity> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsKinematicViscosity,DNVS::MoFa::Units::KinematicViscosity>::ConstructEquivalentQuantity<jsVelocityResponseSpectrum>);
   cls.ImplicitConstructorConversion([](const jsKinematicViscosity& val) {return VelocityResponseSpectrum(val); });
}


