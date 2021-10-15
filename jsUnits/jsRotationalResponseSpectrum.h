//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALRESPONSESPECTRUM_H
#define JSROTATIONALRESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsRotationalResponseSpectrum : public jsTQuantity<jsRotationalResponseSpectrum>
{
public: //internal
    jsRotationalResponseSpectrum(const DNVS::MoFa::Units::RotationalResponseSpectrum& value) : jsTQuantity<jsRotationalResponseSpectrum>(value) {}
    jsRotationalResponseSpectrum(const jsTQuantity<jsRotationalResponseSpectrum>& value) : jsTQuantity<jsRotationalResponseSpectrum>(value) {}
    jsRotationalResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalResponseSpectrum>(value, unit == databaseUnit) {}
    jsRotationalResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsRotationalResponseSpectrum>(value, unitName) {}
    virtual ~jsRotationalResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif