//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTRANSLATIONALDAMPING_H
#define JSTRANSLATIONALDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TranslationalDamping.h>

class JSUNITS_IMPORT_EXPORT jsTranslationalDamping : public jsTQuantity<jsTranslationalDamping>
{
public:
    jsTranslationalDamping(const DNVS::MoFa::Units::TranslationalDamping& value) : jsTQuantity<jsTranslationalDamping>(value) {}
    jsTranslationalDamping(const jsTQuantity<jsTranslationalDamping>& value) : jsTQuantity<jsTranslationalDamping>(value) {}
    jsTranslationalDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTranslationalDamping>(value, unit == databaseUnit) {}
    jsTranslationalDamping(double value, const std::string& unitName) : jsTQuantity<jsTranslationalDamping>(value, unitName) {}
    virtual ~jsTranslationalDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
