//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSROTATIONALQUADRATICDAMPING_H
#define JSROTATIONALQUADRATICDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/RotationalQuadraticDamping.h>

class JSUNITS_IMPORT_EXPORT jsRotationalQuadraticDamping : public jsTQuantity<jsRotationalQuadraticDamping>
{
public:
    jsRotationalQuadraticDamping(const DNVS::MoFa::Units::RotationalQuadraticDamping& value) : jsTQuantity<jsRotationalQuadraticDamping>(value) {}
    jsRotationalQuadraticDamping(const jsTQuantity<jsRotationalQuadraticDamping>& value) : jsTQuantity<jsRotationalQuadraticDamping>(value) {}
    jsRotationalQuadraticDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsRotationalQuadraticDamping>(value, unit == databaseUnit) {}
    jsRotationalQuadraticDamping(double value, const std::string& unitName) : jsTQuantity<jsRotationalQuadraticDamping>(value, unitName) {}
    virtual ~jsRotationalQuadraticDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
