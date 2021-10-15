//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMOMENTRESPONSESPECTRUM_H
#define JSMOMENTRESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MomentResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsMomentResponseSpectrum : public jsTQuantity<jsMomentResponseSpectrum>
{
public: //internal
    jsMomentResponseSpectrum(const DNVS::MoFa::Units::MomentResponseSpectrum& value) : jsTQuantity<jsMomentResponseSpectrum>(value) {}
    jsMomentResponseSpectrum(const jsTQuantity<jsMomentResponseSpectrum>& value) : jsTQuantity<jsMomentResponseSpectrum>(value) {}
    jsMomentResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMomentResponseSpectrum>(value, unit == databaseUnit) {}
    jsMomentResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsMomentResponseSpectrum>(value, unitName) {}
    virtual ~jsMomentResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif