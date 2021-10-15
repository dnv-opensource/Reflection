//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSPRESSURE_H
#define JSPRESSURE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Pressure.h>

class JSUNITS_IMPORT_EXPORT jsPressure : public jsTQuantity<jsPressure>
{
public:
    jsPressure(const DNVS::MoFa::Units::Pressure& value) : jsTQuantity<jsPressure>(value) {}
    jsPressure(const jsTQuantity<jsPressure>& value) : jsTQuantity<jsPressure>(value) {}
    jsPressure(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsPressure>(value, unit == databaseUnit) {}
    jsPressure(double value, const std::string& unitName) : jsTQuantity<jsPressure>(value, unitName) {}
    virtual ~jsPressure() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
