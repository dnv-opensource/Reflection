//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMOMENTOFFORCEENERGYDENSITY_H
#define JSMOMENTOFFORCEENERGYDENSITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MomentOfForceEnergyDensity.h>

class JSUNITS_IMPORT_EXPORT jsMomentOfForceEnergyDensity : public jsTQuantity<jsMomentOfForceEnergyDensity>
{
public: //internal
    jsMomentOfForceEnergyDensity(const DNVS::MoFa::Units::MomentOfForceEnergyDensity& value) : jsTQuantity<jsMomentOfForceEnergyDensity>(value) {}
    jsMomentOfForceEnergyDensity(const jsTQuantity<jsMomentOfForceEnergyDensity>& value) : jsTQuantity<jsMomentOfForceEnergyDensity>(value) {}
    jsMomentOfForceEnergyDensity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMomentOfForceEnergyDensity>(value, unit == databaseUnit) {}
    jsMomentOfForceEnergyDensity(double value, const std::string& unitName) : jsTQuantity<jsMomentOfForceEnergyDensity>(value, unitName) {}
    virtual ~jsMomentOfForceEnergyDensity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif