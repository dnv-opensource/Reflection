//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Math/CompareSignificantDigitsEqual.h"
using namespace DNVS::MoFa::Math;

float AddSignificant(float num,float num2) 
{
    return num + num2;
}

double AddSignificant(double num,double num2) 
{
    return num + num2;
}

TEST(CompareSignificantDigitsEqualTests,FloatComparePlusMinusZero)
{
    EXPECT_TRUE(CompareSignificantDigitsEqual(0.f,-0.f));
}

TEST(CompareSignificantDigitsEqualTests,FloatCompareSmallNumbers)
{
    float num=1e-12f;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5f)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3f)));
}

TEST(CompareSignificantDigitsEqualTests,FloatCompareSmallNegativeNumbers)
{
    float num=-1e-12f;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5f)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3f)));
}

TEST(CompareSignificantDigitsEqualTests,FloatCompareLargeNumbers)
{
    float num=1e34f;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5f)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3f)));
}

TEST(CompareSignificantDigitsEqualTests,FloatCompareLargeNegativeNumbers)
{
    float num=-1e34f;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5f)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3f)));
}

//doubles

TEST(CompareSignificantDigitsEqualTests,DoubleComparePlusMinusZero)
{
    EXPECT_TRUE(CompareSignificantDigitsEqual(0.,-0.));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareSmallNumbers)
{
    double num=1e-12;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareSmallNegativeNumbers)
{
    double num=-1e-12;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareLargeNumbers)
{
    double num=1e34;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareLargeNegativeNumbers)
{
    double num=-1e34;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-5)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num*1.0e-3)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleComparePositiveNumbers5)
{
    double num  =  5.0;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareNegativeNumbers5)
{
    double num  = -5.0;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleComparePositiveNumbers1)
{
    double num  =  1.0010;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareNegativeNumbers1)
{
    double num  = -1.0010;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleComparePositiveNumbers9)
{
    double num  =  9.0;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}

TEST(CompareSignificantDigitsEqualTests,DoubleCompareNegativeNumbers9)
{
    double num  = -9.0;
    double num2 =  0.0004;
    double num3 =  0.0006;
    double num4 = -0.0004;
    double num5 = -0.0006;
    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num2)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num3)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(num,AddSignificant(num, num4)));
    EXPECT_FALSE(CompareSignificantDigitsEqual(num,AddSignificant(num, num5)));

    EXPECT_TRUE(CompareSignificantDigitsEqual(AddSignificant(num, num2),AddSignificant(num, num4)));
}