//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSSECTIONMODULUS_H
#define JSSECTIONMODULUS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/SectionModulus.h>

class JSUNITS_IMPORT_EXPORT jsSectionModulus : public jsTQuantity<jsSectionModulus>
{
public:
    jsSectionModulus(const DNVS::MoFa::Units::SectionModulus& value) : jsTQuantity<jsSectionModulus>(value) {}
    jsSectionModulus(const jsTQuantity<jsSectionModulus>& value) : jsTQuantity<jsSectionModulus>(value) {}
    jsSectionModulus(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsSectionModulus>(value, unit == databaseUnit) {}
    jsSectionModulus(double value, const std::string& unitName) : jsTQuantity<jsSectionModulus>(value, unitName) {}
    virtual ~jsSectionModulus() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
