//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSCOUPLEDQUADRATICDAMPINGMOMENT_H
#define JSCOUPLEDQUADRATICDAMPINGMOMENT_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/CoupledQuadraticDampingMoment.h>

class JSUNITS_IMPORT_EXPORT jsCoupledQuadraticDampingMoment : public jsTQuantity<jsCoupledQuadraticDampingMoment>
{
public:
    jsCoupledQuadraticDampingMoment(const DNVS::MoFa::Units::CoupledQuadraticDampingMoment& value) : jsTQuantity<jsCoupledQuadraticDampingMoment>(value) {}
    jsCoupledQuadraticDampingMoment(const jsTQuantity<jsCoupledQuadraticDampingMoment>& value) : jsTQuantity<jsCoupledQuadraticDampingMoment>(value) {}
    jsCoupledQuadraticDampingMoment(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsCoupledQuadraticDampingMoment>(value, unit == databaseUnit) {}
    jsCoupledQuadraticDampingMoment(double value, const std::string& unitName) : jsTQuantity<jsCoupledQuadraticDampingMoment>(value, unitName) {}
    virtual ~jsCoupledQuadraticDampingMoment() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
