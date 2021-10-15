//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTEMPERATUREDIFFERENCE_H
#define JSTEMPERATUREDIFFERENCE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TempDiff.h>

class JSUNITS_IMPORT_EXPORT jsTemperatureDifference : public jsTQuantity<jsTemperatureDifference>
{
public:
    jsTemperatureDifference(const DNVS::MoFa::Units::TempDiff& value) : jsTQuantity<jsTemperatureDifference>(value) {}
    jsTemperatureDifference(const jsTQuantity<jsTemperatureDifference>& value) : jsTQuantity<jsTemperatureDifference>(value) {}
    jsTemperatureDifference(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTemperatureDifference>(value, unit == databaseUnit) {}
    jsTemperatureDifference(double value, const std::string& unitName) : jsTQuantity<jsTemperatureDifference>(value, unitName) {}
    virtual ~jsTemperatureDifference() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
