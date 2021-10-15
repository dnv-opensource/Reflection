//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDSTIFFNESS_H
#define JSCOUPLEDSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledStiffness.h>

class JSUNITS_IMPORT_EXPORT jsCoupledStiffness : public jsTQuantity<jsCoupledStiffness>
{
public:
    jsCoupledStiffness(const DNVS::MoFa::Units::CoupledStiffness& value) : jsTQuantity<jsCoupledStiffness>(value) {}
    jsCoupledStiffness(const jsTQuantity<jsCoupledStiffness>& value) : jsTQuantity<jsCoupledStiffness>(value) {}
    jsCoupledStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledStiffness>(value, unit == databaseUnit) {}
    jsCoupledStiffness(double value, const std::string& unitName) : jsTQuantity<jsCoupledStiffness>(value, unitName) {}
    virtual ~jsCoupledStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
