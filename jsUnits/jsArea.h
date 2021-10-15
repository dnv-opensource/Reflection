//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSAREA_H
#define JSAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Area.h>

class JSUNITS_IMPORT_EXPORT jsArea : public jsTQuantity<jsArea>
{
public: //internal
    jsArea(const DNVS::MoFa::Units::Area& value) : jsTQuantity<jsArea>(value) {}
    jsArea(const jsTQuantity<jsArea>& value) : jsTQuantity<jsArea>(value) {}
    jsArea(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsArea>(value, unit == databaseUnit) {}
    jsArea(double value, const std::string& unitName) : jsTQuantity<jsArea>(value, unitName) {}
    virtual ~jsArea() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif