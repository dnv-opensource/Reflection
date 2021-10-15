//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Units/Runtime/UnitParser.h"
#include "Units/Mass.h"
#include "Units/Length.h"
#include "Units/Force.h"
#include "Units/TempDiff.h"
#include "Units/Runtime/UnitSystem.h"

using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::Units;
TEST(UnitParserTests, TestParseSimpleUnit)
{
    UnitParser parser;
    EXPECT_EQ(Unit("dm", 0.1, DynamicDimension(1, 0, 0, 0, 0)), parser.Parse("dm"));
}

TEST(UnitParserTests, TestParseMissingWhitespaceOperator)
{
    UnitParser parser;
    EXPECT_EQ(Unit("kN*m", 1000, DynamicDimension(1, 0, 1, 0, 0)), parser.Parse("kNm"));
}

TEST(UnitParserTests, TestParseWithExponent)
{
    UnitParser parser;
    EXPECT_EQ(Unit("dm^2", 0.01, DynamicDimension(2, 0, 0, 0, 0)), parser.Parse("dm2"));
    EXPECT_EQ(Unit("dm^-2", 100, DynamicDimension(-2, 0, 0, 0, 0)), parser.Parse("dm^-2"));
    EXPECT_EQ(Unit("dm^2", 0.01, DynamicDimension(2, 0, 0, 0, 0)), parser.Parse("dm^+2"));
    EXPECT_EQ(Unit(), parser.Parse("dm^"));
}

TEST(UnitParserTests, TestParseMissingWhitespaceOperatorWithExponent)
{
    UnitParser parser;
    EXPECT_EQ(Unit("kN*dm^2", 10, DynamicDimension(2, 0, 1, 0, 0)), parser.Parse("kNdm2"));
}

TEST(UnitParserTests, TestParseMultiplyOperator)
{
    UnitParser parser;
    EXPECT_EQ(Unit("slug*in", (_slug * _in).GetValue(), DynamicDimension(0, 2, 1, 0, 0)), parser.Parse("slug*in"));
}

TEST(UnitParserTests, TestParseDivideOperator)
{
    UnitParser parser;
    EXPECT_EQ(Unit("kg*m/s^2", 1., DynamicDimension(0, 0, 1, 0, 0)), parser.Parse("kg*m/s^2"));
}

TEST(UnitParserTests, TestParseInvalidUnit)
{
    UnitParser parser;
    EXPECT_EQ(Unit(), parser.Parse("]"));
}

TEST(UnitParserTests, TestParseAlmostValidUnit)
{
    UnitParser parser;
    EXPECT_EQ(Unit(), parser.Parse("mini"));
}

TEST(UnitParserTests, TestParseNmPowerNegativeOne)
{
    UnitParser parser;
    auto unit = parser.Parse("Nm^-1");
    unit.SimplifyUnit();
    EXPECT_EQ("N/m", unit.GetUnitName());
}

TEST(UnitParserTests, TestParseZeroExponent)
{
    UnitParser parser;
    EXPECT_EQ(parser.Parse("kN*s^2"), parser.Parse("kN*m^0*s^2"));
}

TEST(UnitParserTests, TestGetUnitFromConversionFactor)
{
    UnitParser parser;
    EXPECT_EQ(parser.Parse("MN"), parser.GetUnitFromConversionFactor(ForceDimension(), 1e6));
    EXPECT_EQ(parser.Parse("mm"), parser.GetUnitFromConversionFactor(LengthDimension(), .001));
    EXPECT_EQ(parser.Parse("delC"), parser.GetUnitFromConversionFactor(TempDiffDimension(), 1));
    EXPECT_FALSE(parser.GetUnitFromConversionFactor(LengthDimension(), 0.05).IsValid());
}