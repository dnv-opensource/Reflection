//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSKINEMATICVISCOSITY_H
#define JSKINEMATICVISCOSITY_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/KinematicViscosity.h>

class JSUNITS_IMPORT_EXPORT jsKinematicViscosity : public jsTQuantity<jsKinematicViscosity>
{
public:
    jsKinematicViscosity(const DNVS::MoFa::Units::KinematicViscosity& value) : jsTQuantity<jsKinematicViscosity>(value) {}
    jsKinematicViscosity(const jsTQuantity<jsKinematicViscosity>& value) : jsTQuantity<jsKinematicViscosity>(value) {}
    jsKinematicViscosity(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsKinematicViscosity>(value, unit == databaseUnit) {}
    jsKinematicViscosity(double value, const std::string& unitName) : jsTQuantity<jsKinematicViscosity>(value, unitName) {}
    virtual ~jsKinematicViscosity() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
