//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSTRANSLATIONALQUADRATICDAMPING_H
#define JSTRANSLATIONALQUADRATICDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/TranslationalQuadraticDamping.h>

class JSUNITS_IMPORT_EXPORT jsTranslationalQuadraticDamping : public jsTQuantity<jsTranslationalQuadraticDamping>
{
public:
    jsTranslationalQuadraticDamping(const DNVS::MoFa::Units::TranslationalQuadraticDamping& value) : jsTQuantity<jsTranslationalQuadraticDamping>(value) {}
    jsTranslationalQuadraticDamping(const jsTQuantity<jsTranslationalQuadraticDamping>& value) : jsTQuantity<jsTranslationalQuadraticDamping>(value) {}
    jsTranslationalQuadraticDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsTranslationalQuadraticDamping>(value, unit == databaseUnit) {}
    jsTranslationalQuadraticDamping(double value, const std::string& unitName) : jsTQuantity<jsTranslationalQuadraticDamping>(value, unitName) {}
    virtual ~jsTranslationalQuadraticDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
