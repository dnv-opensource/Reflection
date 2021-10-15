//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCURVATURE_H
#define JSCURVATURE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/Curvature.h>

class JSUNITS_IMPORT_EXPORT jsCurvature : public jsTQuantity<jsCurvature>
{
public:
    jsCurvature(const DNVS::MoFa::Units::Curvature& value) : jsTQuantity<jsCurvature>(value) {}
    jsCurvature(const jsTQuantity<jsCurvature>& value) : jsTQuantity<jsCurvature>(value) {}
    jsCurvature(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCurvature>(value, unit == databaseUnit) {}
    jsCurvature(double value, const std::string& unitName) : jsTQuantity<jsCurvature>(value, unitName) {}
    virtual ~jsCurvature() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
