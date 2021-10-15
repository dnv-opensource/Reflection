//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTRANSLATIONALSTIFFNESS_H
#define JSTRANSLATIONALSTIFFNESS_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TranslationalStiffness.h>

class JSUNITS_IMPORT_EXPORT jsTranslationalStiffness : public jsTQuantity<jsTranslationalStiffness>
{
public:
    jsTranslationalStiffness(const DNVS::MoFa::Units::TranslationalStiffness& value) : jsTQuantity<jsTranslationalStiffness>(value) {}
    jsTranslationalStiffness(const jsTQuantity<jsTranslationalStiffness>& value) : jsTQuantity<jsTranslationalStiffness>(value) {}
    jsTranslationalStiffness(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTranslationalStiffness>(value, unit == databaseUnit) {}
    jsTranslationalStiffness(double value, const std::string& unitName) : jsTQuantity<jsTranslationalStiffness>(value, unitName) {}
    virtual ~jsTranslationalStiffness() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
