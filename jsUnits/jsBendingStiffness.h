//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSBENDINGSTIFFNESS_H
#define JSBENDINGSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/BendingStiffness.h>

class JSUNITS_IMPORT_EXPORT jsBendingStiffness : public jsTQuantity<jsBendingStiffness>
{
public:
    jsBendingStiffness(const DNVS::MoFa::Units::BendingStiffness& value) : jsTQuantity<jsBendingStiffness>(value) {}
    jsBendingStiffness(const jsTQuantity<jsBendingStiffness>& value) : jsTQuantity<jsBendingStiffness>(value) {}
    jsBendingStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsBendingStiffness>(value, unit == databaseUnit) {}
    jsBendingStiffness(double value, const std::string& unitName) : jsTQuantity<jsBendingStiffness>(value, unitName) {}
    virtual ~jsBendingStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
