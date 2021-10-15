//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsTranslationalDamping.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/TranslationalDamping.h>

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsTranslationalDamping::GetPhenomenon() 
{ 
    return TranslationalDampingPhenomenon();
}

void jsTranslationalDamping::init(jsTypeLibrary& typeLibrary)
{
   jsUnitClass<jsTranslationalDamping,DNVS::MoFa::Units::TranslationalDamping> cls(typeLibrary);
}


