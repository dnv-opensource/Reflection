//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "TypeUtilities/IsSet.h"
#include "TypeUtilities/IsList.h"
#include "TypeUtilities/IsVector.h"
#include "TypeUtilities/IsMap.h"

namespace DNVS { namespace MoFa { namespace TypeUtilities {
	class MySet : public std::set<int> {};
	TEST(IsContainerTests, TestIsSet)
	{
		EXPECT_TRUE(IsSet<std::set<int>>);
		EXPECT_FALSE(IsSet<bool>);
		EXPECT_TRUE(IsSet<MySet>);
		EXPECT_FALSE(IsSet<void>);
	}

	class MyMultiSet : public std::multiset<int> {};
	TEST(IsContainerTests, TestIsMultiSet)
	{
		EXPECT_TRUE(IsMultiSet<std::multiset<int>>);
		EXPECT_FALSE(IsMultiSet<bool>);
		EXPECT_TRUE(IsMultiSet<MyMultiSet>);
		EXPECT_FALSE(IsMultiSet<void>);
	}

	class MyList : public std::list<int> {};
	TEST(IsContainerTests, TestIsList)
	{
		EXPECT_TRUE(IsList<std::list<int>>);
		EXPECT_FALSE(IsList<bool>);
		EXPECT_TRUE(IsList<MyList>);
		EXPECT_FALSE(IsList<void>);
	}

	class MyVector : public std::vector<int> {};
	TEST(IsContainerTests, TestIsVector)
	{
		EXPECT_TRUE(IsVector<std::vector<int>>);
		EXPECT_FALSE(IsVector<bool>);
		EXPECT_TRUE(IsVector<MyVector>);
		EXPECT_FALSE(IsVector<void>);
	}

	class MyMap : public std::map<int,int> {};
	TEST(IsContainerTests, TestIsMap)
	{
		EXPECT_TRUE((IsMap<std::map<int,int>>));
		EXPECT_FALSE(IsMap<bool>);
		EXPECT_TRUE(IsMap<MyMap>);
		EXPECT_TRUE((IsMap<const std::map<int, int>&>));
		EXPECT_FALSE(IsMap<void>);
	}

	class MyMultiMap : public std::multimap<int,int> {};
	TEST(IsContainerTests, TestIsMultiMap)
	{
		EXPECT_TRUE((IsMultiMap<std::multimap<int,int>>));
		EXPECT_FALSE(IsMultiMap<bool>);
		EXPECT_TRUE(IsMultiMap<MyMultiMap>);
		EXPECT_FALSE(IsMultiMap<void>);
	}
} } }