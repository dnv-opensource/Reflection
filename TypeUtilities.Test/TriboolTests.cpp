//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "TypeUtilities\Tribool.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    TEST(TriboolTests, TestIsIndeterminate)
    {
        EXPECT_FALSE(IsIndeterminate(true));
        EXPECT_FALSE(IsIndeterminate(false));
        EXPECT_TRUE(IsIndeterminate(Indeterminate));
    }
    TEST(TriboolTests, TestEqualsComparison)
    {
        Tribool True(true);
        Tribool False(false);
        EXPECT_TRUE(True == true);
        EXPECT_TRUE(true == True);
        EXPECT_TRUE(True == True);
        EXPECT_TRUE(False == False);
        EXPECT_FALSE(False == True);
        EXPECT_FALSE(True == False);
        EXPECT_TRUE(IsIndeterminate(false == Indeterminate));
        EXPECT_TRUE(IsIndeterminate(true == Indeterminate));
        EXPECT_TRUE(IsIndeterminate(Indeterminate == false));
        EXPECT_TRUE(IsIndeterminate(Indeterminate == true));
        EXPECT_TRUE(IsIndeterminate(Indeterminate == Indeterminate));
    }
    TEST(TriboolTests, TestInequalsComparison)
    {
        Tribool True(true);
        Tribool False(false);
        EXPECT_FALSE(True != true);
        EXPECT_FALSE(true != True);
        EXPECT_FALSE(True != True);
        EXPECT_FALSE(False != False);
        EXPECT_TRUE(False != True);
        EXPECT_TRUE(True != False);
        EXPECT_TRUE(IsIndeterminate(false != Indeterminate));
        EXPECT_TRUE(IsIndeterminate(true != Indeterminate));
        EXPECT_TRUE(IsIndeterminate(Indeterminate != false));
        EXPECT_TRUE(IsIndeterminate(Indeterminate != true));
        EXPECT_TRUE(IsIndeterminate(Indeterminate != Indeterminate));
    }

    TEST(TriboolTests, TestLogicalAndOperator)
    {
        Tribool True(true);
        Tribool False(false);
        EXPECT_TRUE(True && true);
        EXPECT_TRUE(true && True);
        EXPECT_TRUE(True && True);
        EXPECT_FALSE(False && False);
        EXPECT_FALSE(False && True);
        EXPECT_FALSE(True && False);
        EXPECT_FALSE(false && Indeterminate);
        EXPECT_TRUE(IsIndeterminate(true && Indeterminate));
        EXPECT_FALSE(Indeterminate && false);
        EXPECT_TRUE(IsIndeterminate(Indeterminate && true));
        EXPECT_TRUE(IsIndeterminate(Indeterminate && Indeterminate));
    }

    TEST(TriboolTests, TestLogicalOrOperator)
    {
        Tribool True(true);
        Tribool False(false);
        EXPECT_TRUE(True || true);
        EXPECT_TRUE(true || True);
        EXPECT_TRUE(True || True);
        EXPECT_FALSE(False || False);
        EXPECT_TRUE(False || True);
        EXPECT_TRUE(True || False);
        EXPECT_TRUE(IsIndeterminate(false || Indeterminate));
        EXPECT_TRUE(true || Indeterminate);
        EXPECT_TRUE(IsIndeterminate(Indeterminate || false));
        EXPECT_TRUE(Indeterminate || true);
        EXPECT_TRUE(IsIndeterminate(Indeterminate || Indeterminate));
    }
}}}