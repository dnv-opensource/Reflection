//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Math/Compare.h"
#include <tuple>

using namespace DNVS::MoFa::Math;
TEST(CompareTests,CompareNotEqual)
{
    typedef std::tuple<double,int,double,float> MyType;
    EXPECT_TRUE(Compare<MyType>(NotEqualComparator(),1.2,4,9.9,3.f));
}

TEST(CompareTests,CompareLess)
{
    typedef std::tuple<double,int,double,float,bool> MyType;
    EXPECT_TRUE(Compare<MyType>(LessThanComparator(),1.2,4,9.9,3.f,true));
}