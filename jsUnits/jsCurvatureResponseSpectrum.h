//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCURVATURERESPONSESPECTRUM_H
#define JSCURVATURERESPONSESPECTRUM_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CurvatureResponseSpectrum.h>

class JSUNITS_IMPORT_EXPORT jsCurvatureResponseSpectrum : public jsTQuantity<jsCurvatureResponseSpectrum>
{
public: //internal
    jsCurvatureResponseSpectrum(const DNVS::MoFa::Units::CurvatureResponseSpectrum& value) : jsTQuantity<jsCurvatureResponseSpectrum>(value) {}
    jsCurvatureResponseSpectrum(const jsTQuantity<jsCurvatureResponseSpectrum>& value) : jsTQuantity<jsCurvatureResponseSpectrum>(value) {}
    jsCurvatureResponseSpectrum(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCurvatureResponseSpectrum>(value, unit == databaseUnit) {}
    jsCurvatureResponseSpectrum(double value, const std::string& unitName) : jsTQuantity<jsCurvatureResponseSpectrum>(value, unitName) {}
    virtual ~jsCurvatureResponseSpectrum() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif