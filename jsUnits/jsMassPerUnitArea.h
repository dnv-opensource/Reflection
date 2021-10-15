//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#ifndef JSMASSPERUNITAREA_H
#define JSMASSPERUNITAREA_H

#include <jsUnits/jsUnits_config.h>
#include <jsUnits/jsTQuantity.h>
#include <Units/MassPerUnitArea.h>

class JSUNITS_IMPORT_EXPORT jsMassPerUnitArea : public jsTQuantity<jsMassPerUnitArea>
{
public:
    jsMassPerUnitArea(const DNVS::MoFa::Units::MassPerUnitArea& value) : jsTQuantity<jsMassPerUnitArea>(value) {}
    jsMassPerUnitArea(const jsTQuantity<jsMassPerUnitArea>& value) : jsTQuantity<jsMassPerUnitArea>(value) {}
    jsMassPerUnitArea(double value = 0, unitType unit = databaseUnit) : jsTQuantity<jsMassPerUnitArea>(value, unit == databaseUnit) {}
    jsMassPerUnitArea(double value, const std::string& unitName) : jsTQuantity<jsMassPerUnitArea>(value, unitName) {}
    virtual ~jsMassPerUnitArea() {}

    static DNVS::MoFa::Units::Runtime::DynamicPhenomenon GetPhenomenon();
    static void init(jsTypeLibrary& typeLibrary);
};

#endif 
