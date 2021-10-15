//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDQUADRATICDAMPINGFORCE_H
#define JSCOUPLEDQUADRATICDAMPINGFORCE_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledQuadraticDampingForce.h>

class JSUNITS_IMPORT_EXPORT jsCoupledQuadraticDampingForce : public jsTQuantity<jsCoupledQuadraticDampingForce>
{
public:
    jsCoupledQuadraticDampingForce(const DNVS::MoFa::Units::CoupledQuadraticDampingForce& value) : jsTQuantity<jsCoupledQuadraticDampingForce>(value) {}
    jsCoupledQuadraticDampingForce(const jsTQuantity<jsCoupledQuadraticDampingForce>& value) : jsTQuantity<jsCoupledQuadraticDampingForce>(value) {}
    jsCoupledQuadraticDampingForce(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledQuadraticDampingForce>(value, unit == databaseUnit) {}
    jsCoupledQuadraticDampingForce(double value, const std::string& unitName) : jsTQuantity<jsCoupledQuadraticDampingForce>(value, unitName) {}
    virtual ~jsCoupledQuadraticDampingForce() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
