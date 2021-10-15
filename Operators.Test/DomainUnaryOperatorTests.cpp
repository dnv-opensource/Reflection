//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include "Operators\UnaryOperators.h"
#include "TestDomain.h"

using namespace DNVS::MoFa::Operators;

TEST(DomainUnaryOperatorTests, CreateDomainAndDoNegate)
{
    Base a;
    UnaryResult<Tags::Negate,Base> result=-a;
	EXPECT_EQ(typeid(UnaryResult<Tags::Negate,Base>), typeid(-a));
}