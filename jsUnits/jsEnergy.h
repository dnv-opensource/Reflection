//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSENERGY_H
#define JSENERGY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Energy.h>

class JSUNITS_IMPORT_EXPORT jsEnergy : public jsTQuantity<jsEnergy>
{
public:
    jsEnergy(const DNVS::MoFa::Units::Energy& value) : jsTQuantity<jsEnergy>(value) {}
    jsEnergy(const jsTQuantity<jsEnergy>& value) : jsTQuantity<jsEnergy>(value) {}
    jsEnergy(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsEnergy>(value, unit == databaseUnit) {}
    jsEnergy(double value, const std::string& unitName) : jsTQuantity<jsEnergy>(value, unitName) {}
    virtual ~jsEnergy() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
