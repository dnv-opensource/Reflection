//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include "Operators\BinaryOperators.h"
#include "TestDomain.h"

using namespace DNVS::MoFa::Operators;

TEST(DomainBinaryOperatorTests, CreateDomainAndDoMultiply)
{
    Base a;
    BinaryResult<Tags::Multiplies,Base,int> result=a*5;
	EXPECT_EQ(typeid(BinaryResult<Tags::Multiplies,Base,int>), typeid(a*5));
    BinaryResult<Tags::Multiplies,int,Base> result2=5*a;
	EXPECT_EQ(typeid(BinaryResult<Tags::Multiplies,int,Base>), typeid(5*a));
    BinaryResult<Tags::Multiplies,Base,Base> result3=a*a;
	EXPECT_EQ(typeid(BinaryResult<Tags::Multiplies,Base,Base>), typeid(a*a));
}