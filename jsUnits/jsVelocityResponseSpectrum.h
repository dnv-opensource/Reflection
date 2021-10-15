//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSVELOCITYRESPONSESPECTRUM_H
#define JSVELOCITYRESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/VelocityResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsVelocityResponseSpectrum : public jsTQuantity<jsVelocityResponseSpectrum>
{
public: //internal
    jsVelocityResponseSpectrum(const DNVS::MoFa::Units::VelocityResponseSpectrum& value) : jsTQuantity<jsVelocityResponseSpectrum>(value) {}
    jsVelocityResponseSpectrum(const jsTQuantity<jsVelocityResponseSpectrum>& value) : jsTQuantity<jsVelocityResponseSpectrum>(value) {}
    jsVelocityResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsVelocityResponseSpectrum>(value, unit == databaseUnit) {}
    jsVelocityResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsVelocityResponseSpectrum>(value, unitName) {}
    virtual ~jsVelocityResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif