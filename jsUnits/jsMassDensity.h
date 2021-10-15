//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMASSDENSITY_H
#define JSMASSDENSITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MassDensity.h>

class JSUNITS_IMPORT_EXPORT jsMassDensity : public jsTQuantity<jsMassDensity>
{
public:
    jsMassDensity(const DNVS::MoFa::Units::MassDensity& value) : jsTQuantity<jsMassDensity>(value) {}
    jsMassDensity(const jsTQuantity<jsMassDensity>& value) : jsTQuantity<jsMassDensity>(value) {}
    jsMassDensity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMassDensity>(value, unit == databaseUnit) {}
    jsMassDensity(double value, const std::string& unitName) : jsTQuantity<jsMassDensity>(value, unitName) {}
    virtual ~jsMassDensity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
