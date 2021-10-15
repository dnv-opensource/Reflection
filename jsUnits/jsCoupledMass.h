//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDMASS_H
#define JSCOUPLEDMASS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledMass.h>

class JSUNITS_IMPORT_EXPORT jsCoupledMass : public jsTQuantity<jsCoupledMass>
{
public:
    jsCoupledMass(const DNVS::MoFa::Units::CoupledMass& value) : jsTQuantity<jsCoupledMass>(value) {}
    jsCoupledMass(const jsTQuantity<jsCoupledMass>& value) : jsTQuantity<jsCoupledMass>(value) {}
    jsCoupledMass(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledMass>(value, unit == databaseUnit) {}
    jsCoupledMass(double value, const std::string& unitName) : jsTQuantity<jsCoupledMass>(value, unitName) {}
    virtual ~jsCoupledMass() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
