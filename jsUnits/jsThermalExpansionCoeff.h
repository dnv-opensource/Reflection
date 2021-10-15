//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTHERMALEXPANSIONCOEFF_H
#define JSTHERMALEXPANSIONCOEFF_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ThermalExpansionCoeff.h>

class JSUNITS_IMPORT_EXPORT jsThermalExpansionCoeff : public jsTQuantity<jsThermalExpansionCoeff>
{
public:
    jsThermalExpansionCoeff(const DNVS::MoFa::Units::ThermalExpansionCoeff& value) : jsTQuantity<jsThermalExpansionCoeff>(value) {}
    jsThermalExpansionCoeff(const jsTQuantity<jsThermalExpansionCoeff>& value) : jsTQuantity<jsThermalExpansionCoeff>(value) {}
    jsThermalExpansionCoeff(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsThermalExpansionCoeff>(value, unit == databaseUnit) {}
    jsThermalExpansionCoeff(double value, const std::string& unitName) : jsTQuantity<jsThermalExpansionCoeff>(value, unitName) {}
    virtual ~jsThermalExpansionCoeff() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
