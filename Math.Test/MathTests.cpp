//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Math\Math.h"
#include "boost\assign\list_of.hpp"
#include <list>
#include <vector>
#include "gtest/gtest.h"
#include "GoogleTest\Main.h"

using namespace DNVS::MoFa::Math::Statistics;
TEST(MathTests,MedianFromVectorEven)
{
     std::vector<double> vec=  boost::assign::list_of(5)(1)(2)(1);
     double median= CalculateMedian(vec);
     EXPECT_EQ(median,(2+1)/2.0);
}

TEST(MathTests,MedianFromListOdd)
{
    std::list<long> lis=  boost::assign::list_of(5)(1)(2);
    long median= CalculateMedian(lis);
    EXPECT_EQ(median,2);
}

TEST(MathTests,MedianFromSetOdd)
{
    std::set<double> vec=  boost::assign::list_of(5)(1)(2);
    double median= CalculateMedian(vec);
    EXPECT_EQ(median,2.0);
}