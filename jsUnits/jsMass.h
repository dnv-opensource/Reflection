//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMASS_H
#define JSMASS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Mass.h>

class JSUNITS_IMPORT_EXPORT jsMass : public jsTQuantity<jsMass>
{
public:
    jsMass(const DNVS::MoFa::Units::Mass& value) : jsTQuantity<jsMass>(value) {}
    jsMass(const jsTQuantity<jsMass>& value) : jsTQuantity<jsMass>(value) {}
    jsMass(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMass>(value, unit == databaseUnit) {}
    jsMass(double value, const std::string& unitName) : jsTQuantity<jsMass>(value, unitName) {}
    virtual ~jsMass() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
