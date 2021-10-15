//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSDISPLACEMENTRESPONSESPECTRUM_H
#define JSDISPLACEMENTRESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/DisplacementResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsDisplacementResponseSpectrum : public jsTQuantity<jsDisplacementResponseSpectrum>
{
public: //internal
    jsDisplacementResponseSpectrum(const DNVS::MoFa::Units::DisplacementResponseSpectrum& value) : jsTQuantity<jsDisplacementResponseSpectrum>(value) {}
    jsDisplacementResponseSpectrum(const jsTQuantity<jsDisplacementResponseSpectrum>& value) : jsTQuantity<jsDisplacementResponseSpectrum>(value) {}
    jsDisplacementResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsDisplacementResponseSpectrum>(value, unit == databaseUnit) {}
    jsDisplacementResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsDisplacementResponseSpectrum>(value, unitName) {}
    virtual ~jsDisplacementResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif