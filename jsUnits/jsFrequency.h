//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFREQUENCY_H
#define JSFREQUENCY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Frequency.h>

class JSUNITS_IMPORT_EXPORT jsFrequency : public jsTQuantity<jsFrequency>
{
public:
    jsFrequency(const DNVS::MoFa::Units::Frequency& value) : jsTQuantity<jsFrequency>(value) {}
    jsFrequency(const jsTQuantity<jsFrequency>& value) : jsTQuantity<jsFrequency>(value) {}
    jsFrequency(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsFrequency>(value, unit == databaseUnit) {}
    jsFrequency(double value, const std::string& unitName) : jsTQuantity<jsFrequency>(value, unitName) {}
    virtual ~jsFrequency() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
