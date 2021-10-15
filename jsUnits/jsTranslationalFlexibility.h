//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTRANSLATIONALFLEXIBILITY_H
#define JSTRANSLATIONALFLEXIBILITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TranslationalFlexibility.h>

class JSUNITS_IMPORT_EXPORT jsTranslationalFlexibility : public jsTQuantity<jsTranslationalFlexibility>
{
public:
    jsTranslationalFlexibility(const DNVS::MoFa::Units::TranslationalFlexibility& value) : jsTQuantity<jsTranslationalFlexibility>(value) {}
    jsTranslationalFlexibility(const jsTQuantity<jsTranslationalFlexibility>& value) : jsTQuantity<jsTranslationalFlexibility>(value) {}
    jsTranslationalFlexibility(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTranslationalFlexibility>(value, unit == databaseUnit) {}
    jsTranslationalFlexibility(double value, const std::string& unitName) : jsTQuantity<jsTranslationalFlexibility>(value, unitName) {}
    virtual ~jsTranslationalFlexibility() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
