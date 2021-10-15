//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSACCELERATION_H
#define JSACCELERATION_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Acceleration.h>

class JSUNITS_IMPORT_EXPORT jsAcceleration : public jsTQuantity<jsAcceleration>
{
public: //internal
    jsAcceleration(const DNVS::MoFa::Units::Acceleration& value) : jsTQuantity<jsAcceleration>(value) {}
    jsAcceleration(const jsTQuantity<jsAcceleration>& value) : jsTQuantity<jsAcceleration>(value) {}
    jsAcceleration(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsAcceleration>(value, unit == databaseUnit) {}
    jsAcceleration(double value, const std::string& unitName) : jsTQuantity<jsAcceleration>(value, unitName) {}
    virtual ~jsAcceleration() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif