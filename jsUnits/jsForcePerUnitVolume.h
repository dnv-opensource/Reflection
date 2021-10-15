//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCEPERUNITVOLUME_H
#define JSFORCEPERUNITVOLUME_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForcePerUnitVolume.h>

class JSUNITS_IMPORT_EXPORT jsForcePerUnitVolume : public jsTQuantity<jsForcePerUnitVolume>
{
public:
    jsForcePerUnitVolume(const DNVS::MoFa::Units::ForcePerUnitVolume& value) : jsTQuantity<jsForcePerUnitVolume>(value) {}
    jsForcePerUnitVolume(const jsTQuantity<jsForcePerUnitVolume>& value) : jsTQuantity<jsForcePerUnitVolume>(value) {}
    jsForcePerUnitVolume(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForcePerUnitVolume>(value, unit == databaseUnit) {}
    jsForcePerUnitVolume(double value, const std::string& unitName) : jsTQuantity<jsForcePerUnitVolume>(value, unitName) {}
    virtual ~jsForcePerUnitVolume() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
