//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSAXIALSTIFFNESS_H
#define JSAXIALSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include "Units/AxialStiffness.h"

class JSUNITS_IMPORT_EXPORT jsAxialStiffness : public jsTQuantity<jsAxialStiffness>
{
public:
    jsAxialStiffness(const DNVS::MoFa::Units::AxialStiffness& axialStiffness) : jsTQuantity<jsAxialStiffness>(axialStiffness) {}
    jsAxialStiffness(const jsTQuantity<jsAxialStiffness>& value) : jsTQuantity<jsAxialStiffness>(value) {}
    jsAxialStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsAxialStiffness>(value, unit == databaseUnit) {}
    jsAxialStiffness(double value, const std::string& unitName) : jsTQuantity<jsAxialStiffness>(value, unitName) {}
    virtual ~jsAxialStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
