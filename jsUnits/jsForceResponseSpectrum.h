//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSFORCERESPONSESPECTRUM_H
#define JSFORCERESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/ForceResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsForceResponseSpectrum : public jsTQuantity<jsForceResponseSpectrum>
{
public: //internal
    jsForceResponseSpectrum(const DNVS::MoFa::Units::ForceResponseSpectrum& value) : jsTQuantity<jsForceResponseSpectrum>(value) {}
    jsForceResponseSpectrum(const jsTQuantity<jsForceResponseSpectrum>& value) : jsTQuantity<jsForceResponseSpectrum>(value) {}
    jsForceResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsForceResponseSpectrum>(value, unit == databaseUnit) {}
    jsForceResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsForceResponseSpectrum>(value, unitName) {}
    virtual ~jsForceResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif