//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDDAMPING_H
#define JSCOUPLEDDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledDamping.h>

class JSUNITS_IMPORT_EXPORT jsCoupledDamping : public jsTQuantity<jsCoupledDamping>
{
public:
    jsCoupledDamping(const DNVS::MoFa::Units::CoupledDamping& value) : jsTQuantity<jsCoupledDamping>(value) {}
    jsCoupledDamping(const jsTQuantity<jsCoupledDamping>& value) : jsTQuantity<jsCoupledDamping>(value) {}
    jsCoupledDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledDamping>(value, unit == databaseUnit) {}
    jsCoupledDamping(double value, const std::string& unitName) : jsTQuantity<jsCoupledDamping>(value, unitName) {}
    virtual ~jsCoupledDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
