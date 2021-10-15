//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsSectionModulus.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/SectionModulus.h>
#include "jsVolume.h"
#include "jsFirstMomentOfArea.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsSectionModulus::GetPhenomenon() 
{ 
    return SectionModulusPhenomenon();
}

void jsSectionModulus::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsSectionModulus,DNVS::MoFa::Units::SectionModulus> cls(typeLibrary);
   if(cls.reinit()) return;
   cls.ImplicitConstructorConversion(&jsUnitClass<jsSectionModulus,DNVS::MoFa::Units::SectionModulus>::ConstructEquivalentQuantity<jsVolume>);
   cls.ImplicitConstructorConversion(&jsUnitClass<jsSectionModulus,DNVS::MoFa::Units::SectionModulus>::ConstructEquivalentQuantity<jsFirstMomentOfArea>);      
   cls.ImplicitConstructorConversion([](const jsSectionModulus& val) {return Volume(val); });
   cls.ImplicitConstructorConversion([](const jsSectionModulus& val) {return FirstMomentOfArea(val); });
}


