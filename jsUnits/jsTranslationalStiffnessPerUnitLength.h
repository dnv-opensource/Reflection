//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTRANSLATIONALSTIFFNESSPERUNITLENGTH_H
#define JSTRANSLATIONALSTIFFNESSPERUNITLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TranslationalStiffnessPerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsTranslationalStiffnessPerUnitLength : public jsTQuantity<jsTranslationalStiffnessPerUnitLength>
{
public:
    jsTranslationalStiffnessPerUnitLength(const DNVS::MoFa::Units::TranslationalStiffnessPerUnitLength& value) : jsTQuantity<jsTranslationalStiffnessPerUnitLength>(value) {}
    jsTranslationalStiffnessPerUnitLength(const jsTQuantity<jsTranslationalStiffnessPerUnitLength>& value) : jsTQuantity<jsTranslationalStiffnessPerUnitLength>(value) {}
    jsTranslationalStiffnessPerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTranslationalStiffnessPerUnitLength>(value, unit == databaseUnit) {}
    jsTranslationalStiffnessPerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsTranslationalStiffnessPerUnitLength>(value, unitName) {}
    virtual ~jsTranslationalStiffnessPerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
