//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFIRSTMOMENTOFAREA_H
#define JSFIRSTMOMENTOFAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/FirstMomentOfArea.h>

class JSUNITS_IMPORT_EXPORT jsFirstMomentOfArea : public jsTQuantity<jsFirstMomentOfArea>
{
public:
    jsFirstMomentOfArea(const DNVS::MoFa::Units::FirstMomentOfArea& value) : jsTQuantity<jsFirstMomentOfArea>(value) {}
    jsFirstMomentOfArea(const jsTQuantity<jsFirstMomentOfArea>& value) : jsTQuantity<jsFirstMomentOfArea>(value) {}
    jsFirstMomentOfArea(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsFirstMomentOfArea>(value, unit == databaseUnit) {}
    jsFirstMomentOfArea(double value, const std::string& unitName) : jsTQuantity<jsFirstMomentOfArea>(value, unitName) {}
    virtual ~jsFirstMomentOfArea() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
