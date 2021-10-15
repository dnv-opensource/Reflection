//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVOLUME_H
#define JSVOLUME_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Volume.h>

class JSUNITS_IMPORT_EXPORT jsVolume : public jsTQuantity<jsVolume>
{
public:
    jsVolume(const DNVS::MoFa::Units::Volume& value) : jsTQuantity<jsVolume>(value) {}
    jsVolume(const jsTQuantity<jsVolume>& value) : jsTQuantity<jsVolume>(value) {}
    jsVolume(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsVolume>(value, unit == databaseUnit) {}
    jsVolume(double value, const std::string& unitName) : jsTQuantity<jsVolume>(value, unitName) {}
    virtual ~jsVolume() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
