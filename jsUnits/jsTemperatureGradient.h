#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TempGradient.h>

class JSUNITS_IMPORT_EXPORT jsTemperatureGradient : public jsTQuantity<jsTemperatureGradient>
{
public:
    jsTemperatureGradient(const DNVS::MoFa::Units::TempGradient& value) : jsTQuantity<jsTemperatureGradient>(value) {}
    jsTemperatureGradient(const jsTQuantity<jsTemperatureGradient>& value) : jsTQuantity<jsTemperatureGradient>(value) {}
    jsTemperatureGradient(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTemperatureGradient>(value, unit == databaseUnit) {}
    jsTemperatureGradient(double value, const std::string& unitName) : jsTQuantity<jsTemperatureGradient>(value, unitName) {}
    virtual ~jsTemperatureGradient() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};
