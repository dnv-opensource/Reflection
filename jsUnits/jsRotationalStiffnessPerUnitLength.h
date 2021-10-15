//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALSTIFFNESSPERUNITLENGTH_H
#define JSROTATIONALSTIFFNESSPERUNITLENGTH_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalStiffnessPerUnitLength.h>

class JSUNITS_IMPORT_EXPORT jsRotationalStiffnessPerUnitLength : public jsTQuantity<jsRotationalStiffnessPerUnitLength>
{
public:
    jsRotationalStiffnessPerUnitLength(const DNVS::MoFa::Units::RotationalStiffnessPerUnitLength& value) : jsTQuantity<jsRotationalStiffnessPerUnitLength>(value) {}
    jsRotationalStiffnessPerUnitLength(const jsTQuantity<jsRotationalStiffnessPerUnitLength>& value) : jsTQuantity<jsRotationalStiffnessPerUnitLength>(value) {}
    jsRotationalStiffnessPerUnitLength(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalStiffnessPerUnitLength>(value, unit == databaseUnit) {}
    jsRotationalStiffnessPerUnitLength(double value, const std::string& unitName) : jsTQuantity<jsRotationalStiffnessPerUnitLength>(value, unitName) {}
    virtual ~jsRotationalStiffnessPerUnitLength() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
