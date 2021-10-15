//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <jsUnits/jsForce.h>
#include <jsUnits/jsUnitClass.h>
#include <Units/Force.h>
#include "jsRotationalStiffnessPerUnitLength.h"
#include "jsCoupledStiffness.h"
#include "jsAxialStiffness.h"

using namespace DNVS::MoFa::Units;
Runtime::DynamicPhenomenon jsForce::GetPhenomenon()
{
    return ForcePhenomenon();
}

void jsForce::init(jsTypeLibrary& typeLibrary)
{
    jsUnitClass<jsForce, DNVS::MoFa::Units::Force> cls(typeLibrary);
    if (cls.reinit()) return;
    cls.ImplicitConstructorConversion(&jsUnitClass<jsForce, DNVS::MoFa::Units::Force>::ConstructEquivalentQuantity<jsRotationalStiffnessPerUnitLength>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsForce, DNVS::MoFa::Units::Force>::ConstructEquivalentQuantity<jsCoupledStiffness>);
    cls.ImplicitConstructorConversion(&jsUnitClass<jsForce, DNVS::MoFa::Units::Force>::ConstructEquivalentQuantity<jsAxialStiffness>);
    cls.ImplicitConstructorConversion([](const jsForce& val) {return RotationalStiffnessPerUnitLength(val); });
    cls.ImplicitConstructorConversion([](const jsForce& val) {return CoupledStiffness(val); });
    cls.ImplicitConstructorConversion([](const jsForce& val) {return AxialStiffness(val); });

    std::string spacer10 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    std::string spacer12 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    std::string spacer14 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    std::string spacer16 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    std::string spacer18 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
    std::string spacer20 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";

    auto& example = cls.AddExample("Force");
    example.AddScript("<b>Force units</b>" + spacer16 + "<b>Conversion factor</b>");
    example.AddScript("N (SI base unit)&nbsp;" + spacer18 + "= " + std::string(mofa::lexical_cast<std::string>(_N.GetValue())));
    example.AddScript("kgf (kilogram force)" + spacer14 + "= " + std::string(mofa::lexical_cast<std::string>(_kgf.GetValue())));
    example.AddScript("lbf (pound force)&nbsp;" + spacer18 + "= " + std::string(mofa::lexical_cast<std::string>(_lbf.GetValue())));
    example.AddScript("kipf (kilopound force)&nbsp;" + spacer12 + "= " + std::string(mofa::lexical_cast<std::string>(_kipf.GetValue())));
}
