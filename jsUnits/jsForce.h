//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCE_H
#define JSFORCE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Force.h>

class JSUNITS_IMPORT_EXPORT jsForce : public jsTQuantity<jsForce>
{
public:
    jsForce(const DNVS::MoFa::Units::Force& value) : jsTQuantity<jsForce>(value) {}
    jsForce(const jsTQuantity<jsForce>& value) : jsTQuantity<jsForce>(value) {}
    jsForce(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForce>(value, unit == databaseUnit) {}
    jsForce(double value, const std::string& unitName) : jsTQuantity<jsForce>(value, unitName) {}
    virtual ~jsForce() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
