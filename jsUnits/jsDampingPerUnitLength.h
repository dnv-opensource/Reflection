//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSDAMPINGPERUNITLENGTH_H
#define JSDAMPINGPERUNITLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/DampingPerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsDampingPerUnitLength : public jsTQuantity<jsDampingPerUnitLength>
{
public:
    jsDampingPerUnitLength(const DNVS::MoFa::Units::DampingPerUnitLength& value) : jsTQuantity<jsDampingPerUnitLength>(value) {}
    jsDampingPerUnitLength(const jsTQuantity<jsDampingPerUnitLength>& value) : jsTQuantity<jsDampingPerUnitLength>(value) {}
    jsDampingPerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsDampingPerUnitLength>(value, unit == databaseUnit) {}
    jsDampingPerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsDampingPerUnitLength>(value, unitName) {}
    virtual ~jsDampingPerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
