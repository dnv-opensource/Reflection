//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVELOCITY_H
#define JSVELOCITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Velocity.h>

class JSUNITS_IMPORT_EXPORT jsVelocity : public jsTQuantity<jsVelocity>
{
public:
    jsVelocity(const DNVS::MoFa::Units::Velocity& value) : jsTQuantity<jsVelocity>(value) {}
    jsVelocity(const jsTQuantity<jsVelocity>& value) : jsTQuantity<jsVelocity>(value) {}
    jsVelocity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsVelocity>(value, unit == databaseUnit) {}
    jsVelocity(double value, const std::string& unitName) : jsTQuantity<jsVelocity>(value, unitName) {}
    virtual ~jsVelocity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
