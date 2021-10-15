//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "Units/Runtime/UnitManager.h"
#include "Units/Length.h"
#include "Units/Velocity.h"

using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::Units;

TEST(UnitManagerTests, AddUnit)
{
    UnitManager manager;
    manager.AddUnit(_in);
    EXPECT_NE(manager.end(), manager.find("in"));
    EXPECT_EQ(manager.end(), manager.find("m"));
}

TEST(UnitManagerTests, AddSIUnit)
{
    UnitManager manager;
    manager.AddSIUnit(_m);
    ASSERT_NE(manager.end(), manager.find("cm"));
    EXPECT_DOUBLE_EQ(0.01, manager.find("cm")->second.GetConversionFactor());
}

TEST(UnitManagerTests, AddAllSIUnit)
{
    UnitManager manager;
    manager.AddAllSIUnits();
    ASSERT_NE(manager.end(), manager.find("MHz"));
    EXPECT_DOUBLE_EQ(1e6, manager.find("MHz")->second.GetConversionFactor());
}

TEST(UnitManagerTests, AddImperialUnits)
{
    UnitManager manager;
    manager.AddImperialUnits();
    ASSERT_NE(manager.end(), manager.find("Knot"));
    EXPECT_DOUBLE_EQ(_Knot.GetValue(), manager.find("Knot")->second.GetConversionFactor());
    ASSERT_EQ(manager.end(), manager.find("MKnot"));
}