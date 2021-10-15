//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsVolume.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Volume.h>
#include "jsSectionModulus.h"
#include "jsFirstMomentOfArea.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsVolume::GetPhenomenon() 
{ 
    return VolumePhenomenon();
}

void jsVolume::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsVolume,DNVS::MoFa::Units::Volume> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsVolume,DNVS::MoFa::Units::Volume>::ConstructEquivalentQuantity<jsSectionModulus>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsVolume,DNVS::MoFa::Units::Volume>::ConstructEquivalentQuantity<jsFirstMomentOfArea>);      
   cls.ImplicitConstructorConversion([](const jsVolume& val) {return SectionModulus(val); });
   cls.ImplicitConstructorConversion([](const jsVolume& val) {return FirstMomentOfArea(val); });
}


