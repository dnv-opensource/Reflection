//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALDAMPING_H
#define JSROTATIONALDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalDamping.h>

class JSUNITS_IMPORT_EXPORT jsRotationalDamping : public jsTQuantity<jsRotationalDamping>
{
public:
    jsRotationalDamping(const DNVS::MoFa::Units::RotationalDamping& value) : jsTQuantity<jsRotationalDamping>(value) {}
    jsRotationalDamping(const jsTQuantity<jsRotationalDamping>& value) : jsTQuantity<jsRotationalDamping>(value) {}
    jsRotationalDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalDamping>(value, unit == databaseUnit) {}
    jsRotationalDamping(double value, const std::string& unitName) : jsTQuantity<jsRotationalDamping>(value, unitName) {}
    virtual ~jsRotationalDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
