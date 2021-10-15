//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#pragma once


#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/SpecificHeat.h>

class JSUNITS_IMPORT_EXPORT jsSpecificHeat : public jsTQuantity<jsSpecificHeat>
{
public: //internal
    jsSpecificHeat(const DNVS::MoFa::Units::SpecificHeat& value) : jsTQuantity<jsSpecificHeat>(value) {}
    jsSpecificHeat(const jsTQuantity<jsSpecificHeat>& value) : jsTQuantity<jsSpecificHeat>(value) {}
    jsSpecificHeat(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsSpecificHeat>(value, unit == databaseUnit) {}
    jsSpecificHeat(double value, const std::string& unitName) : jsTQuantity<jsSpecificHeat>(value, unitName) {}
    virtual ~jsSpecificHeat() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};
