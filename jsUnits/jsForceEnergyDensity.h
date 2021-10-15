//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCEENERGYDENSITY_H
#define JSFORCEENERGYDENSITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForceEnergyDensity.h>

class JSUNITS_IMPORT_EXPORT jsForceEnergyDensity : public jsTQuantity<jsForceEnergyDensity>
{
public: //internal
    jsForceEnergyDensity(const DNVS::MoFa::Units::ForceEnergyDensity& value) : jsTQuantity<jsForceEnergyDensity>(value) {}
    jsForceEnergyDensity(const jsTQuantity<jsForceEnergyDensity>& value) : jsTQuantity<jsForceEnergyDensity>(value) {}
    jsForceEnergyDensity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForceEnergyDensity>(value, unit == databaseUnit) {}
    jsForceEnergyDensity(double value, const std::string& unitName) : jsTQuantity<jsForceEnergyDensity>(value, unitName) {}
    virtual ~jsForceEnergyDensity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif