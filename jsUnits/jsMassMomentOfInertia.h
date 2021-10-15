//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMASSMOMENTOFINERTIA_H
#define JSMASSMOMENTOFINERTIA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MassMomentOfInertia.h>

class JSUNITS_IMPORT_EXPORT jsMassMomentOfInertia : public jsTQuantity<jsMassMomentOfInertia>
{
public:
    jsMassMomentOfInertia(const DNVS::MoFa::Units::MassMomentOfInertia& value) : jsTQuantity<jsMassMomentOfInertia>(value) {}
    jsMassMomentOfInertia(const jsTQuantity<jsMassMomentOfInertia>& value) : jsTQuantity<jsMassMomentOfInertia>(value) {}
    jsMassMomentOfInertia(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMassMomentOfInertia>(value, unit == databaseUnit) {}
    jsMassMomentOfInertia(double value, const std::string& unitName) : jsTQuantity<jsMassMomentOfInertia>(value, unitName) {}
    virtual ~jsMassMomentOfInertia() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
