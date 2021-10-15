//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSENERGYDESITY_H
#define JSENERGYDESITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/EnergyDensity.h>

class JSUNITS_IMPORT_EXPORT jsEnergyDensity : public jsTQuantity<jsEnergyDensity>
{
public:
    jsEnergyDensity(const DNVS::MoFa::Units::EnergyDensity& value) : jsTQuantity<jsEnergyDensity>(value) {}
    jsEnergyDensity(const jsTQuantity<jsEnergyDensity>& value) : jsTQuantity<jsEnergyDensity>(value) {}
    jsEnergyDensity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsEnergyDensity>(value, unit == databaseUnit) {}
    jsEnergyDensity(double value, const std::string& unitName) : jsTQuantity<jsEnergyDensity>(value, unitName) {}
    virtual ~jsEnergyDensity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
