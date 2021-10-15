//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSECONDMOMENTOFAREA_H
#define JSSECONDMOMENTOFAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/SecondMomentOfArea.h>

class JSUNITS_IMPORT_EXPORT jsSecondMomentOfArea : public jsTQuantity<jsSecondMomentOfArea>
{
public:
    jsSecondMomentOfArea(const DNVS::MoFa::Units::SecondMomentOfArea& value) : jsTQuantity<jsSecondMomentOfArea>(value) {}
    jsSecondMomentOfArea(const jsTQuantity<jsSecondMomentOfArea>& value) : jsTQuantity<jsSecondMomentOfArea>(value) {}
    jsSecondMomentOfArea(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsSecondMomentOfArea>(value, unit == databaseUnit) {}
    jsSecondMomentOfArea(double value, const std::string& unitName) : jsTQuantity<jsSecondMomentOfArea>(value, unitName) {}
    virtual ~jsSecondMomentOfArea() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
