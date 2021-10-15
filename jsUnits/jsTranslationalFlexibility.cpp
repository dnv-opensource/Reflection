//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTranslationalFlexibility.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TranslationalFlexibility.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTranslationalFlexibility::GetPhenomenon() 
{ 
    return TranslationalFlexibilityPhenomenon();
}

void jsTranslationalFlexibility::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTranslationalFlexibility,DNVS::MoFa::Units::TranslationalFlexibility> cls(typeLibrary);
}

