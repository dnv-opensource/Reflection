//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDQUADRATICDAMPING_H
#define JSCOUPLEDQUADRATICDAMPING_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledQuadraticDamping.h>

class JSUNITS_IMPORT_EXPORT jsCoupledQuadraticDamping : public jsTQuantity<jsCoupledQuadraticDamping>
{
public:
    jsCoupledQuadraticDamping(const DNVS::MoFa::Units::CoupledQuadraticDamping& value) : jsTQuantity<jsCoupledQuadraticDamping>(value) {}
    jsCoupledQuadraticDamping(const jsTQuantity<jsCoupledQuadraticDamping>& value) : jsTQuantity<jsCoupledQuadraticDamping>(value) {}
    jsCoupledQuadraticDamping(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledQuadraticDamping>(value, unit == databaseUnit) {}
    jsCoupledQuadraticDamping(double value, const std::string& unitName) : jsTQuantity<jsCoupledQuadraticDamping>(value, unitName) {}
    virtual ~jsCoupledQuadraticDamping() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
