//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Units/Length.h"
#include "Units/Mass.h"
#include "Units/Time.h"

#include "Units/Runtime/Unit.h"

using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;

TEST(RuntimeUnitTests, CreateDynamicUnit)
{
    Unit a("cm", _cm);
    EXPECT_DOUBLE_EQ(0.01, a.GetConversionFactor());
    EXPECT_EQ("cm", a.GetUnitName());
    EXPECT_EQ(DynamicDimension(1, 0, 0, 0, 0), a.GetDimension());
}

TEST(RuntimeUnitTests, MultiplyDynamicUnits)
{
    Unit a(_cm);
    Unit b(_m);
    Unit c = a * b;
    EXPECT_DOUBLE_EQ(0.01, c.GetConversionFactor());
    EXPECT_EQ("cm*m", c.GetUnitName());
    EXPECT_EQ(DynamicDimension(2, 0, 0, 0, 0), c.GetDimension());
}

TEST(RuntimeUnitTests, PowDynamicUnits)
{
    Unit a(_cm);
    Unit c = Pow(a, 3);
    EXPECT_DOUBLE_EQ(0.000001, c.GetConversionFactor());
    EXPECT_EQ("cm^3", c.GetUnitName());
    EXPECT_EQ(DynamicDimension(3, 0, 0, 0, 0), c.GetDimension());
}

TEST(RuntimeUnitTests, PowDynamicCombinedUnits)
{
    Unit a(_cm);
    Unit b(_m);
    Unit c = Pow(a * b, 2);
    EXPECT_DOUBLE_EQ(0.0001, c.GetConversionFactor());
    EXPECT_EQ("cm^2*m^2", c.GetUnitName());
    EXPECT_EQ(DynamicDimension(4, 0, 0, 0, 0), c.GetDimension());
}

TEST(RuntimeUnitTests, DivideDynamicUnits)
{
    Unit a(_m);
    Unit b(_cm);
    Unit c = a / b;
    EXPECT_DOUBLE_EQ(100, c.GetConversionFactor());
    EXPECT_EQ("m/cm", c.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 0, 0, 0, 0), c.GetDimension());
}

TEST(RuntimeUnitTests, PowDivideDynamicUnits)
{
    Unit a(_kg);
    Unit b(_cm);
    Unit c = Pow(a / b, -2);
    EXPECT_DOUBLE_EQ(0.0001, c.GetConversionFactor());
    EXPECT_EQ("kg^-2*cm^2", c.GetUnitName());
    EXPECT_EQ(DynamicDimension(4, -4, -2, 0, 0), c.GetDimension());
}

TEST(RuntimeUnitTests, CreateFundamentalUnit)
{
    Unit unit(_kg);
    EXPECT_DOUBLE_EQ(1.0, unit.GetConversionFactor());
    EXPECT_EQ("kg", unit.GetUnitName());
    EXPECT_EQ(DynamicDimension(-1, 2, 1, 0, 0), unit.GetDimension());
}

TEST(RuntimeUnitTests, MultiplyUnitsTest)
{
    Unit mass(_kg);
    Unit length(_m);
    Unit result = mass * length;
    EXPECT_DOUBLE_EQ(1.0, result.GetConversionFactor());
    EXPECT_EQ("kg*m", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 2, 1, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, DivideUnitsTest)
{
    Unit length(_m);
    Unit time(_hr);
    Unit result = length / time;
    EXPECT_DOUBLE_EQ(1. / 3600., result.GetConversionFactor());
    EXPECT_EQ("m/hr", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(1, -1, 0, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, ComplexDivideTest)
{
    Unit length(_m);
    Unit time(_min);
    Unit mass(_kg);
    Unit result = mass / (length*time);
    EXPECT_DOUBLE_EQ(1. / 60, result.GetConversionFactor());
    EXPECT_EQ("kg/m/min", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(-2, 1, 1, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, PowTest)
{
    Unit time(_s);
    Unit result = Pow(time, 2);
    EXPECT_DOUBLE_EQ(1.0, result.GetConversionFactor());
    EXPECT_EQ("s^2", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 2, 0, 0, 0), result.GetDimension());
}


TEST(RuntimeUnitTests, InvertedPowTest)
{
    Unit length(_m);
    Unit time(_s);
    Unit mass(_kg);
    Unit result = mass * length / Pow(time, 2);
    EXPECT_DOUBLE_EQ(1., result.GetConversionFactor());
    EXPECT_EQ("kg*m/s^2", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 0, 1, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, PowOfCombined)
{
    Unit length(_m);
    Unit time(_s);
    Unit mass(_kg);
    Unit result = Pow(mass * length / Pow(time, 2), 2);
    EXPECT_DOUBLE_EQ(1., result.GetConversionFactor());
    EXPECT_EQ("kg^2*m^2/s^4", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 0, 2, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, NegativePow)
{
    Unit length(_m);
    Unit time(_s);
    Unit mass(_kg);
    Unit result = Pow(mass * length / Pow(time, 2), -2);
    EXPECT_DOUBLE_EQ(1., result.GetConversionFactor());
    EXPECT_EQ("kg^-2/m^2*s^4", result.GetUnitName());
    EXPECT_EQ(DynamicDimension(0, 0, -2, 0, 0), result.GetDimension());
}

TEST(RuntimeUnitTests, AddExponentToLastTerm)
{
    Unit length(_m);
    Unit mass(_kg);
    Unit result = AddExponentToLastTerm(length*mass,2);
    EXPECT_EQ(length*Pow(mass,2),result);
}