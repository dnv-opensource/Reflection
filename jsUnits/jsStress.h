//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSTRESS_H
#define JSSTRESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Stress.h>

class JSUNITS_IMPORT_EXPORT jsStress : public jsTQuantity<jsStress>
{
public:
    jsStress(const DNVS::MoFa::Units::Stress& value) : jsTQuantity<jsStress>(value) {}
    jsStress(const jsTQuantity<jsStress>& value) : jsTQuantity<jsStress>(value) {}
    jsStress(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsStress>(value, unit == databaseUnit) {}
    jsStress(double value, const std::string& unitName) : jsTQuantity<jsStress>(value, unitName) {}
    virtual ~jsStress() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
