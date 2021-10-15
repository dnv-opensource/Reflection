//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALFLEXIBILITY_H
#define JSROTATIONALFLEXIBILITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalFlexibility.h>

class JSUNITS_IMPORT_EXPORT jsRotationalFlexibility : public jsTQuantity<jsRotationalFlexibility>
{
public:
    jsRotationalFlexibility(const DNVS::MoFa::Units::RotationalFlexibility& value) : jsTQuantity<jsRotationalFlexibility>(value) {}
    jsRotationalFlexibility(const jsTQuantity<jsRotationalFlexibility>& value) : jsTQuantity<jsRotationalFlexibility>(value) {}
    jsRotationalFlexibility(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalFlexibility>(value, unit == databaseUnit) {}
    jsRotationalFlexibility(double value, const std::string& unitName) : jsTQuantity<jsRotationalFlexibility>(value, unitName) {}
    virtual ~jsRotationalFlexibility() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
