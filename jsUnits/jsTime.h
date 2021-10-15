//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTIME_H
#define JSTIME_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Time.h>

class JSUNITS_IMPORT_EXPORT jsTime : public jsTQuantity<jsTime>
{
public:
    jsTime(const DNVS::MoFa::Units::Time& value) : jsTQuantity<jsTime>(value) {}
    jsTime(const jsTQuantity<jsTime>& value) : jsTQuantity<jsTime>(value) {}
    jsTime(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTime>(value, unit == databaseUnit) {}
    jsTime(double value, const std::string& unitName) : jsTQuantity<jsTime>(value, unitName) {}
    virtual ~jsTime() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
