//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALVELOCITY_H
#define JSROTATIONALVELOCITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalVelocity.h>

class JSUNITS_IMPORT_EXPORT jsRotationalVelocity : public jsTQuantity<jsRotationalVelocity>
{
public:
    jsRotationalVelocity(const DNVS::MoFa::Units::RotationalVelocity& value) : jsTQuantity<jsRotationalVelocity>(value) {}
    jsRotationalVelocity(const jsTQuantity<jsRotationalVelocity>& value) : jsTQuantity<jsRotationalVelocity>(value) {}
    jsRotationalVelocity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalVelocity>(value, unit == databaseUnit) {}
    jsRotationalVelocity(double value, const std::string& unitName) : jsTQuantity<jsRotationalVelocity>(value, unitName) {}
    virtual ~jsRotationalVelocity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
