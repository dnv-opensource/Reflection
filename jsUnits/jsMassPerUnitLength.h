//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMASSPERUNITLENGTH_H
#define JSMASSPERUNITLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MassPerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsMassPerUnitLength : public jsTQuantity<jsMassPerUnitLength>
{
public:
    jsMassPerUnitLength(const DNVS::MoFa::Units::MassPerUnitLength& value) : jsTQuantity<jsMassPerUnitLength>(value) {}
    jsMassPerUnitLength(const jsTQuantity<jsMassPerUnitLength>& value) : jsTQuantity<jsMassPerUnitLength>(value) {}
    jsMassPerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMassPerUnitLength>(value, unit == databaseUnit) {}
    jsMassPerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsMassPerUnitLength>(value, unitName) {}
    virtual ~jsMassPerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
