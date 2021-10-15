//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALSTIFFNESS_H
#define JSROTATIONALSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalStiffness.h>

class JSUNITS_IMPORT_EXPORT jsRotationalStiffness : public jsTQuantity<jsRotationalStiffness>
{
public:
    jsRotationalStiffness(const DNVS::MoFa::Units::RotationalStiffness& value) : jsTQuantity<jsRotationalStiffness>(value) {}
    jsRotationalStiffness(const jsTQuantity<jsRotationalStiffness>& value) : jsTQuantity<jsRotationalStiffness>(value) {}
    jsRotationalStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalStiffness>(value, unit == databaseUnit) {}
    jsRotationalStiffness(double value, const std::string& unitName) : jsTQuantity<jsRotationalStiffness>(value, unitName) {}
    virtual ~jsRotationalStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
