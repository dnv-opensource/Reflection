//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Units\Runtime\UnitComposer.h"
#include "Units\Runtime\DefaultUnitProvider.h"
#include "Units\Energy.h"
#include "Units\MomentOfForce.h"
#include "Units\TempDiff.h"
#include "Units\Mass.h"
#include "Units\Runtime\UnitParser.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Test {
    TEST(UnitComposerTests, ComposeFromQuantity)
    {
        UnitParser parser;
        auto provider = std::make_shared<DefaultUnitProvider>(parser.Parse("m"), parser.Parse("kN"), parser.Parse("delC"));
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit<Energy>().GetUnitName());
        EXPECT_EQ("kN*m", UnitComposer(provider).GetUnit<MomentOfForce>().GetUnitName());
        EXPECT_EQ("delC", UnitComposer(provider).GetUnit<TempDiff>().GetUnitName());
        EXPECT_EQ("tonne", UnitComposer(provider).GetUnit<Mass>().GetUnitName());
    }

    TEST(UnitComposerTests, ComposeFromDeducedQuantity)
    {
        UnitParser parser;
        auto provider = std::make_shared<DefaultUnitProvider>(parser.Parse("m"), parser.Parse("kN"), parser.Parse("delC"));
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit<decltype(-4._J)>().GetUnitName());
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit<decltype(4._kN*_m)>().GetUnitName());
        EXPECT_EQ("tonne", UnitComposer(provider).GetUnit<decltype(-4._kg)>().GetUnitName());
    }

    TEST(UnitComposerTests, ComposeFromDynamicQuantity)
    {
        UnitParser parser;
        auto provider = std::make_shared<DefaultUnitProvider>(parser.Parse("m"), parser.Parse("kN"), parser.Parse("delC"));
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit(DynamicPhenomenon(EnergyPhenomenon())).GetUnitName());
        EXPECT_EQ("kN*m", UnitComposer(provider).GetUnit(DynamicPhenomenon(MomentOfForcePhenomenon())).GetUnitName());
        EXPECT_EQ("delC", UnitComposer(provider).GetUnit(DynamicPhenomenon(TempDiffPhenomenon())).GetUnitName());
        EXPECT_EQ("tonne", UnitComposer(provider).GetUnit(DynamicPhenomenon(MassPhenomenon())).GetUnitName());
    }

    TEST(UnitComposerTests, ComposeFromDynamicDimension)
    {
        UnitParser parser;
        auto provider = std::make_shared<DefaultUnitProvider>(parser.Parse("m"), parser.Parse("kN"), parser.Parse("delC"));
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit(DynamicDimension(EnergyDimension())).GetUnitName());
        EXPECT_EQ("kJ", UnitComposer(provider).GetUnit(DynamicDimension(MomentOfForceDimension())).GetUnitName());
        EXPECT_EQ("tonne", UnitComposer(provider).GetUnit(DynamicDimension(MassDimension())).GetUnitName());
    }
}}}}}