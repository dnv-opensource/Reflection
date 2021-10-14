//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities\IsConstructible.h"
#include "gtest\gtest.h"
namespace DNVS {namespace MoFa {namespace TypeUtilities { namespace Tests {
    class A0 {
    public:
        A0();
    };

    class A1 {
    public:
        A1(int a);
    };

    class A2 {
    public:
        A2(int a, int b);
    };

    class A3 {
    public:
        A3(int a, int b, int c);
    };

    TEST(IsConstructibleTests, TestConstructible0)
    {
        EXPECT_TRUE(IsConstructible<A0>::value);
        EXPECT_FALSE(IsConstructible<A1>::value);
        EXPECT_FALSE(IsConstructible<A2>::value);
        EXPECT_FALSE(IsConstructible<A3>::value);
    }

    TEST(IsConstructibleTests, TestConstructible1)
    {
        EXPECT_FALSE((IsConstructible<A0, int>::value));
        EXPECT_TRUE((IsConstructible<A1, int>::value));
        EXPECT_FALSE((IsConstructible<A2, int>::value));
        EXPECT_FALSE((IsConstructible<A3, int>::value));
    }

    TEST(IsConstructibleTests, TestConstructible2)
    {
        EXPECT_FALSE((IsConstructible<A0, int, int>::value));
        EXPECT_FALSE((IsConstructible<A1, int, int>::value));
        EXPECT_TRUE((IsConstructible<A2, int, int>::value));
        EXPECT_FALSE((IsConstructible<A3, int, int>::value));
    }

    TEST(IsConstructibleTests, TestConstructible3)
    {
        EXPECT_FALSE((IsConstructible<A0, int, int, int>::value));
        EXPECT_FALSE((IsConstructible<A1, int, int, int>::value));
        EXPECT_FALSE((IsConstructible<A2, int, int, int>::value));
        EXPECT_TRUE((IsConstructible<A3, int, int, int>::value));
    }
}}}}