//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "TypeUtilities/RangeSet.h"
using namespace DNVS::MoFa::TypeUtilities;
TEST(RangeSetTests, InsertOneRange_Ok)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    ASSERT_EQ(range.size(), 1);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
}

TEST(RangeSetTests, InsertOneRangeReverse_Ok)
{
    RangeSet<double> range;
    range.emplace(4, 0);
    ASSERT_EQ(range.size(), 1);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
}

TEST(RangeSetTests, InsertTwoDisconnectedRanges)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    range.emplace(5, 8);
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
    EXPECT_EQ(range.back(), std::make_pair(5., 8.));
}

TEST(RangeSetTests, InsertTwoDisconnectedRangesOppositeOrder)
{
    RangeSet<double> range;
    range.emplace(5, 8);
    range.emplace(0, 4);
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
    EXPECT_EQ(range.back(), std::make_pair(5., 8.));
}

TEST(RangeSetTests, InsertTwoJoinedRangesMerge)
{
    RangeSet<double> range;
    range.emplace(4, 8);
    range.emplace(0, 4);
    ASSERT_EQ(range.size(), 1);
    EXPECT_EQ(range.front(), std::make_pair(0., 8.));
}

TEST(RangeSetTests, InsertTwoDisconnectedRangesThenOverlap_JoinsAll)
{
    RangeSet<double> range;
    range.emplace(5, 8);
    range.emplace(0, 4);
    range.emplace(-1, 9);
    ASSERT_EQ(range.size(), 1);
    EXPECT_EQ(range.front(), std::make_pair(-1., 9.));
}

TEST(RangeSetTests, InsertThreeDisconnectedRangesThenJoinLastTwo)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    range.emplace(5, 6);
    range.emplace(7, 8);
    range.emplace(6, 7.1);
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
    EXPECT_EQ(range.back(), std::make_pair(5., 8.));
}

TEST(RangeSetTests, InsertRange_RemoveRange_MakeHole)
{
    RangeSet<double> range;
    range.emplace(0, 8);
    range.erase(std::make_pair(6, 4));
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(0., 4.));
    EXPECT_EQ(range.back(), std::make_pair(6., 8.));
}

TEST(RangeSetTests, InsertRanges_ShortenFirst)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    range.emplace(5, 8);
    range.erase(std::make_pair(0, 2));
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(2., 4.));
    EXPECT_EQ(range.back(), std::make_pair(5., 8.));
}

TEST(RangeSetTests, InsertRanges_ShortenFirstEnd)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    range.emplace(5, 8);
    range.erase(std::make_pair(2, 6));
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front(), std::make_pair(0., 2.));
    EXPECT_EQ(range.back(), std::make_pair(6., 8.));
}

TEST(RangeSetTests, InsertRanges_RemoveFirst)
{
    RangeSet<double> range;
    range.emplace(0, 4);
    range.emplace(5, 8);
    range.erase(std::make_pair(0, 5));
    ASSERT_EQ(range.size(), 1);
    EXPECT_EQ(range.back(), std::make_pair(5., 8.));
}

TEST(RangeSetTests, ConainsTests)
{
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }.Contains(RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) })));
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }.Contains(RangeSet<double>())));
    EXPECT_FALSE((RangeSet<double>().Contains(RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) })));
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }.Contains(RangeSet<double>{ std::make_pair(2, 3) })));
    EXPECT_FALSE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }.Contains(RangeSet<double>{ std::make_pair(-5, -2) })));
    EXPECT_FALSE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }.Contains(RangeSet<double>{ std::make_pair(-5, 2) })));
}

TEST(RangeSetTests, ConainsValue)
{
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(2));
    EXPECT_FALSE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(4.5));
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(8));
}

TEST(RangeSetTests, ConainsRange)
{
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(std::make_pair(2, 3)));
    EXPECT_FALSE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(std::make_pair(2, 5)));
    EXPECT_FALSE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(std::make_pair(4.1, 4.8)));
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(std::make_pair(5, 7)));
    EXPECT_TRUE((RangeSet<double>{ std::make_pair(0, 4), std::make_pair(5, 8) }).Contains(std::make_pair(5, 8)));
}