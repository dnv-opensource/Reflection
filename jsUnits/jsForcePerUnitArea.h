//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCEPERUNITAREA_H
#define JSFORCEPERUNITAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForcePerUnitArea.h>

class JSUNITS_IMPORT_EXPORT jsForcePerUnitArea : public jsTQuantity<jsForcePerUnitArea>
{
public:
    jsForcePerUnitArea(const DNVS::MoFa::Units::ForcePerUnitArea& value) : jsTQuantity<jsForcePerUnitArea>(value) {}
    jsForcePerUnitArea(const jsTQuantity<jsForcePerUnitArea>& value) : jsTQuantity<jsForcePerUnitArea>(value) {}
    jsForcePerUnitArea(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForcePerUnitArea>(value, unit == databaseUnit) {}
    jsForcePerUnitArea(double value, const std::string& unitName) : jsTQuantity<jsForcePerUnitArea>(value, unitName) {}
    virtual ~jsForcePerUnitArea() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
