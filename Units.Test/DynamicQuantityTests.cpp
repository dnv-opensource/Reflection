//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Units/Runtime/DynamicQuantity.h"
#include "Units/Length.h"
#include "Units/Force.h"
#include "Units/Mass.h"
#include "Units/Time.h"
#include "Units/Runtime/DefaultUnitProvider.h"
#include "Units/Runtime/UnitSystem.h"
#include "Units/Formatting/DefaultUnitFormatter.h"
#include "Formatting/FormattingService.h"
#include "Units/Formatting/IUnitFormatter.h"
#include "Units/RotationalStiffness.h"

using namespace DNVS::MoFa;
using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::Units::Formatting;
using namespace DNVS::MoFa::Formatting;

TEST(DynamicQuantityTests, CreateFromLength)
{
    DynamicQuantity a(5, _m);
    EXPECT_DOUBLE_EQ(5, a.GetValue());
}

TEST(DynamicQuantityTests, MultiplyDimensions)
{
    DynamicQuantity a(5, _m);
    DynamicQuantity  b = a * DynamicQuantity(7, Pow(Unit(_m), 2));
    EXPECT_DOUBLE_EQ(5 * 7, b.GetValue());
    EXPECT_EQ(DynamicDimension(3, 0, 0, 0, 0), b.GetSimplifiedUnit().GetDimension());
}

TEST(DynamicQuantityTests, AddDimensions_Incompatible_Throws)
{
    DynamicQuantity a(5, _m);
    EXPECT_THROW(DynamicQuantity b = a + DynamicQuantity(7, Pow(Unit(_m), 2)), std::runtime_error);
}

TEST(DynamicQuantityTests, AddDimensions_DifferentLengths)
{
    DynamicQuantity a(5, _m);
    DynamicQuantity b(10, _cm);
    DynamicQuantity c = a + b;
    EXPECT_DOUBLE_EQ(5.1, c.GetValue());
}

TEST(DynamicQuantityTests, SubDimensions_Incompatible_Throws)
{
    DynamicQuantity a(5, _m);
    EXPECT_THROW(DynamicQuantity  b = a-DynamicQuantity(7, _kN), std::runtime_error);
}

TEST(DynamicQuantityTests, AddQuantities_Ok)
{
    DynamicQuantity a(5, _m);
    EXPECT_EQ(DynamicQuantity(10, _m), a + a);
}

TEST(DynamicQuantityTests, AsQuantity)
{
    DynamicQuantity myMass(5, _kg);
    EXPECT_EQ(5 * _kg, myMass.As<Mass>());
    EXPECT_THROW(myMass.As<Time>(), std::runtime_error);
}

TEST(DynamicQuantityTests, Power)
{
    DynamicQuantity myLength(5, _m);
    EXPECT_EQ(DynamicQuantity(125., Pow(Unit(_m), 3)), Pow(myLength, 3));
}

TEST(DynamicQuantityTests, Compare_NotEqual_DifferentUnits)
{
    DynamicQuantity myLength(5, _m);
    EXPECT_NE(myLength, Pow(myLength, 2));
}

TEST(DynamicQuantityTests, Compare_NotEqual_DifferentValues)
{
    DynamicQuantity myLength(5, _m);
    DynamicQuantity myLength2(5.5, _m);
    EXPECT_NE(myLength, myLength2);
}


TEST(DynamicQuantityTests, AddAndAssignQuantity)
{
    DynamicQuantity length(5, _m);
    length += DynamicQuantity(8.4, _m);
    EXPECT_EQ(DynamicQuantity(13.4, _m), length);
}

TEST(DynamicQuantityTests, DivideQuantities)
{
    DynamicQuantity force(5, _kN);
    DynamicQuantity length(8.8, _m);
    DynamicQuantity result = force / length;
    EXPECT_EQ(DynamicQuantity(5 / 8.8, Unit(_kN) / Unit(_m)), result);
}

TEST(DynamicQuantityTests, DivideMassByMassDensity_SimplifyResultingUnit)
{
    DynamicQuantity mass(5, _kg);
    DynamicQuantity length(1, _m);
    DynamicQuantity density = mass / Pow(length, 3);
    DynamicQuantity volume = mass / density;

    EXPECT_EQ("m^3", volume.GetSimplifiedUnit().GetUnitName());
}

TEST(DynamicQuantityTests, DivideMeterByKilometer_SimplifyResultingUnitToUnitLess)
{
    DynamicQuantity longLength(1, _km);
    DynamicQuantity shortLength(5, _m);
    DynamicQuantity unitLess = shortLength / longLength;

    EXPECT_TRUE(unitLess.GetUnit().IsValid());
    EXPECT_EQ(Unit::GetEmptyUnit(), unitLess.GetUnit());
    EXPECT_EQ(0.005, unitLess.GetValue());
}

TEST(DynamicQuantityTests, TestNameOfDynamicPhenomenon)
{
    EXPECT_EQ(DynamicPhenomenon(RotationalStiffnessPhenomenon()).GetName(), "RotationalStiffness");
}

TEST(DynamicQuantityTests, TestConversionOfRotationalStiffnessToString)
{
    EXPECT_EQ(DynamicPhenomenon(RotationalStiffnessPhenomenon()).GetName(), "RotationalStiffness");
    FormattingService service;
    service.AddFormatter<IUnitFormatter>(std::make_shared<DefaultUnitFormatter>(UnitSystem("mm", "N")));
    EXPECT_EQ(ToString(RotationalStiffness(15), service), "15000 N*mm");
    EXPECT_EQ(ToString(DynamicQuantity(RotationalStiffness(15)), service), "15000 N*mm");
}
