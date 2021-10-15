//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALACCELERATION_H
#define JSROTATIONALACCELERATION_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalAcceleration.h>

class JSUNITS_IMPORT_EXPORT jsRotationalAcceleration : public jsTQuantity<jsRotationalAcceleration>
{
public:
    jsRotationalAcceleration(const DNVS::MoFa::Units::RotationalAcceleration& value) : jsTQuantity<jsRotationalAcceleration>(value) {}
    jsRotationalAcceleration(const jsTQuantity<jsRotationalAcceleration>& value) : jsTQuantity<jsRotationalAcceleration>(value) {}
    jsRotationalAcceleration(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalAcceleration>(value, unit == databaseUnit) {}
    jsRotationalAcceleration(double value, const std::string& unitName) : jsTQuantity<jsRotationalAcceleration>(value, unitName) {}
    virtual ~jsRotationalAcceleration() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
