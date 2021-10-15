//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Math/CompareAlmostEqual.h"
using namespace DNVS::MoFa::Math;

TEST(CompareAlmostEqualTests,FloatComparePlusMinusZero)
{
    EXPECT_TRUE(CompareAlmostEqual(0.f,-0.f));
}

float AddUlps(float num,int ulps) {
    FloatingPointAsInteger<float> intRep(num);
    intRep.AddUlps(ulps);
    return intRep.ToFloatingPoint();
}

double AddUlps(double num,int ulps) {
    FloatingPointAsInteger<double> intRep(num);
    intRep.AddUlps(ulps);
    return intRep.ToFloatingPoint();
}

TEST(CompareAlmostEqualTests,FloatCompareSmallNumbers)
{
    float num=1e-12f;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,FloatCompareSmallNegativeNumbers)
{
    float num=-1e-12f;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,FloatCompareLargeNumbers)
{
    float num=1e34f;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,FloatCompareLargeNegativeNumbers)
{
    float num=-1e34f;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

//doubles

TEST(CompareAlmostEqualTests,DoubleComparePlusMinusZero)
{
    EXPECT_TRUE(CompareAlmostEqual(0.,-0.));
}

TEST(CompareAlmostEqualTests,DoubleCompareSmallNumbers)
{
    double num=1e-12;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,DoubleCompareSmallNegativeNumbers)
{
    double num=-1e-12;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,DoubleCompareLargeNumbers)
{
    double num=1e34;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}

TEST(CompareAlmostEqualTests,DoubleCompareLargeNegativeNumbers)
{
    double num=-1e34;
    EXPECT_TRUE(CompareAlmostEqual(num,AddUlps(num,3)));
    EXPECT_FALSE(CompareAlmostEqual(num,AddUlps(num,5)));
}