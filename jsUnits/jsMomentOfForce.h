//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMOMENTOFFORCE_H
#define JSMOMENTOFFORCE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MomentOfForce.h>

class JSUNITS_IMPORT_EXPORT jsMomentOfForce : public jsTQuantity<jsMomentOfForce>
{
public:
    jsMomentOfForce(const DNVS::MoFa::Units::MomentOfForce& value) : jsTQuantity<jsMomentOfForce>(value) {}
    jsMomentOfForce(const jsTQuantity<jsMomentOfForce>& value) : jsTQuantity<jsMomentOfForce>(value) {}
    jsMomentOfForce(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMomentOfForce>(value, unit == databaseUnit) {}
    jsMomentOfForce(double value, const std::string& unitName) : jsTQuantity<jsMomentOfForce>(value, unitName) {}
    virtual ~jsMomentOfForce() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
