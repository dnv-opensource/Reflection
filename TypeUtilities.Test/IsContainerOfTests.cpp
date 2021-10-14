//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "TypeUtilities\IsContainerOf.h"
#include "TypeUtilities\Where.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    TEST(IsContainerOfTests,SetOfIntIsContainerOfDouble)
    {
        static_assert(IsContainerOf<std::set<int>,double>::value, "std::set<int> should be a container of double");
        static_assert(!IsContainerOf<int,double>::value, "int should not be a container of double");
    }

    int TestContainerOfInt(...) {return 0;}
    template<typename ContainerT>
    int TestContainerOfInt(const ContainerT& container, WHERE(IsContainerOf<ContainerT, int>::value))
    {
        return 1;
    }

    TEST(IsContainerOfTests, EnableFunctionIfIsContainerOf)
    {
        std::set<int> a;
        EXPECT_EQ(1,TestContainerOfInt(a));
        EXPECT_EQ(0,TestContainerOfInt(1));
        std::set<std::string> b;
        EXPECT_EQ(0,TestContainerOfInt(b));
    }
}}}