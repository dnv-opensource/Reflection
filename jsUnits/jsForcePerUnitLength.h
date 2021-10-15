//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCEPERUNITLENGTH_H
#define JSFORCEPERUNITLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForcePerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsForcePerUnitLength : public jsTQuantity<jsForcePerUnitLength>
{
public:
    jsForcePerUnitLength(const DNVS::MoFa::Units::ForcePerUnitLength& value) : jsTQuantity<jsForcePerUnitLength>(value) {}
    jsForcePerUnitLength(const jsTQuantity<jsForcePerUnitLength>& value) : jsTQuantity<jsForcePerUnitLength>(value) {}
    jsForcePerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForcePerUnitLength>(value, unit == databaseUnit) {}
    jsForcePerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsForcePerUnitLength>(value, unitName) {}
    virtual ~jsForcePerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
