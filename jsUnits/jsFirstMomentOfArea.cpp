//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsFirstMomentOfArea.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/FirstMomentOfArea.h>
#include "jsSectionModulus.h"
#include "jsVolume.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsFirstMomentOfArea::GetPhenomenon()
{
    return FirstMomentOfAreaPhenomenon();
}

void jsFirstMomentOfArea::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsFirstMomentOfArea, FirstMomentOfArea> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsFirstMomentOfArea, FirstMomentOfArea>::ConstructEquivalentQuantity<jsSectionModulus>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsFirstMomentOfArea, FirstMomentOfArea>::ConstructEquivalentQuantity<jsVolume>);
    cls.ImplicitConstructorConversion([](const jsFirstMomentOfArea& val) {return SectionModulus(val); });
    cls.ImplicitConstructorConversion([](const jsFirstMomentOfArea& val) {return Volume(val); });

}


