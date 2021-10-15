//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSQuadraticDampingPerUnitLength_H
#define JSQuadraticDampingPerUnitLength_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/QuadraticDampingPerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsQuadraticDampingPerUnitLength : public jsTQuantity<jsQuadraticDampingPerUnitLength>
{
public:
    jsQuadraticDampingPerUnitLength(const DNVS::MoFa::Units::QuadraticDampingPerUnitLength& value) : jsTQuantity<jsQuadraticDampingPerUnitLength>(value) {}
    jsQuadraticDampingPerUnitLength(const jsTQuantity<jsQuadraticDampingPerUnitLength>& value) : jsTQuantity<jsQuadraticDampingPerUnitLength>(value) {}
    jsQuadraticDampingPerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsQuadraticDampingPerUnitLength>(value, unit == databaseUnit) {}
    jsQuadraticDampingPerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsQuadraticDampingPerUnitLength>(value, unitName) {}
    virtual ~jsQuadraticDampingPerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
