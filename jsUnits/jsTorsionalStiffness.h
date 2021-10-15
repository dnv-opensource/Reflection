//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTORSIONALSTIFFNESS_H
#define JSTORSIONALSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TorsionalStiffness.h>

class JSUNITS_IMPORT_EXPORT jsTorsionalStiffness : public jsTQuantity<jsTorsionalStiffness>
{
public:
    jsTorsionalStiffness(const DNVS::MoFa::Units::TorsionalStiffness& value) : jsTQuantity<jsTorsionalStiffness>(value) {}
    jsTorsionalStiffness(const jsTQuantity<jsTorsionalStiffness>& value) : jsTQuantity<jsTorsionalStiffness>(value) {}
    jsTorsionalStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTorsionalStiffness>(value, unit == databaseUnit) {}
    jsTorsionalStiffness(double value, const std::string& unitName) : jsTQuantity<jsTorsionalStiffness>(value, unitName) {}
    virtual ~jsTorsionalStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
