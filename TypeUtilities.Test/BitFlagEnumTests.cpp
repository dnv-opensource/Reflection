//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities\BitFlagEnum.h"
#include "gtest\gtest.h"

using namespace DNVS::MoFa::TypeUtilities;
class TestEnumClass : public BitFlagEnum<TestEnumClass>
{
    friend struct Base;
    TestEnumClass(int v) : Base(v) {}
public:
    static const TestEnumClass EnumValue1;
    static const TestEnumClass EnumValue2;
};

const TestEnumClass TestEnumClass::EnumValue1=1;
const TestEnumClass TestEnumClass::EnumValue2=2;

TEST(BitFlagEnumTests,ConstructBitFlag)
{
    TestEnumClass a(TestEnumClass::EnumValue1);
    EXPECT_EQ(a,TestEnumClass::EnumValue1);
}

TEST(BitFlagEnumTests,OrBitFlags)
{
    TestEnumClass a(TestEnumClass::EnumValue1);
    EXPECT_FALSE(a.Has(TestEnumClass::EnumValue2));
    a|=TestEnumClass::EnumValue2;
    EXPECT_TRUE(a.Has(TestEnumClass::EnumValue1));
    EXPECT_TRUE(a.Has(TestEnumClass::EnumValue2));
}

TEST(BitFlagEnumTests,AndBitFlags)
{
    TestEnumClass a(TestEnumClass::EnumValue1);
    EXPECT_TRUE(a.Has(TestEnumClass::EnumValue1));
    a&=TestEnumClass::EnumValue2;
    EXPECT_FALSE(a.Has(TestEnumClass::EnumValue1));
    EXPECT_FALSE(a.Has(TestEnumClass::EnumValue2));
}

TEST(BitFlagEnumTests,RemoveBitFlag)
{
    TestEnumClass a(TestEnumClass::EnumValue1|TestEnumClass::EnumValue2);
    a.Remove(TestEnumClass::EnumValue1);
    EXPECT_EQ(TestEnumClass::EnumValue2,a);
}
