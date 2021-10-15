//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONPERLENGTH_H
#define JSROTATIONPERLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationPerLength.h>

class JSUNITS_IMPORT_EXPORT jsRotationPerLength : public jsTQuantity<jsRotationPerLength>
{
public:
    jsRotationPerLength(const DNVS::MoFa::Units::RotationPerLength& value) : jsTQuantity<jsRotationPerLength>(value) {}
    jsRotationPerLength(const jsTQuantity<jsRotationPerLength>& value) : jsTQuantity<jsRotationPerLength>(value) {}
    jsRotationPerLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationPerLength>(value, unit == databaseUnit) {}
    jsRotationPerLength(double value, const std::string& unitName) : jsTQuantity<jsRotationPerLength>(value, unitName) {}
    virtual ~jsRotationPerLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
