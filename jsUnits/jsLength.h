//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSLENGTH_H
#define JSLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Length.h>

class JSUNITS_IMPORT_EXPORT jsLength : public jsTQuantity<jsLength>
{
public:
    jsLength(const DNVS::MoFa::Units::Length& value) : jsTQuantity<jsLength>(value) {}
    jsLength(const jsTQuantity<jsLength>& value) : jsTQuantity<jsLength>(value) {}
    jsLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsLength>(value, unit == databaseUnit) {}
    jsLength(double value, const std::string& unitName) : jsTQuantity<jsLength>(value, unitName) {}
    virtual ~jsLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
