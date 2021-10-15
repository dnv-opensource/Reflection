//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSANGLE_H
#define JSANGLE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Angle.h>

class JSUNITS_IMPORT_EXPORT jsAngle : public jsTQuantity<jsAngle>
{
public: //internal
    jsAngle(const DNVS::MoFa::Units::Angle& value) : jsTQuantity<jsAngle>(value) {}
    jsAngle(const jsTQuantity<jsAngle>& value) : jsTQuantity<jsAngle>(value) {}
    jsAngle(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsAngle>(value, unit == databaseUnit) {}
    jsAngle(double value, const std::string& unitName) : jsTQuantity<jsAngle>(value, unitName) {}
    virtual ~jsAngle() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif