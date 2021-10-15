//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef AFX_JSFORCEPERUNITAREAPERANGLE_H
#define AFX_JSFORCEPERUNITAREAPERANGLE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForcePerUnitAreaPerAngle.h>

class JSUNITS_IMPORT_EXPORT jsForcePerUnitAreaPerAngle : public jsTQuantity<jsForcePerUnitAreaPerAngle>
{
public:
    jsForcePerUnitAreaPerAngle(const DNVS::MoFa::Units::ForcePerUnitAreaPerAngle& value) : jsTQuantity<jsForcePerUnitAreaPerAngle>(value) {}
    jsForcePerUnitAreaPerAngle(const jsTQuantity<jsForcePerUnitAreaPerAngle>& value) : jsTQuantity<jsForcePerUnitAreaPerAngle>(value) {}
    jsForcePerUnitAreaPerAngle(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForcePerUnitAreaPerAngle>(value, unit == databaseUnit) {}
    jsForcePerUnitAreaPerAngle(double value, const std::string& unitName) : jsTQuantity<jsForcePerUnitAreaPerAngle>(value, unitName) {}
    virtual ~jsForcePerUnitAreaPerAngle() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);

};

#endif
