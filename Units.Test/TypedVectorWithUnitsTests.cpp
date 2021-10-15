//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeUtilities/TypedVector.h"
#include "Units/Length.h"
#include "Units/Mass.h"
#include "Units/Area.h"
#include "Units/Math.h"
#include "Units/Runtime/DynamicQuantity.h"
#include "gtest/gtest.h"

using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;
using namespace DNVS::MoFa::TypeUtilities;


TEST(TypedTypedVectorWithUnitsTests, TestTypedVectorOfLength)
{
    TypedVector<Length, 3> TypedVector(1_m, 2_m, 3_m);
    EXPECT_EQ(TypedVector.GetZ(), 3_m);
}

TEST(TypedTypedVectorWithUnitsTests, TestTypedVectorOfDynamicQuantity)
{
    TypedVector<DynamicQuantity, 3> vector(1_m, 2_m, DynamicQuantity(1));
    TypedVector<Length, 3> lengthTypedVector = vector;
    EXPECT_EQ(lengthTypedVector.GetZ(), 1_m);
}

TEST(TypedTypedVectorWithUnitsTests, TestInvalidConversionOfDifferentQuantitiesToLength)
{
    TypedVector<DynamicQuantity, 3> vector(1_m, 2_m, 3_kg);
    using LengthTypedVector = TypedVector<Length, 3>;
    EXPECT_THROW(LengthTypedVector lengthVector = vector, std::exception);
}

TEST(TypedTypedVectorWithUnitsTests, ConvertFromLengthTypedVectorToDynamicQuantityTypedVector)
{
    TypedVector<Length, 3> vector(1_m, 2_m, 3_m);
    TypedVector<DynamicQuantity, 3> dq = vector;
    EXPECT_EQ(dq.GetX(), 1_m);
}

TEST(TypedTypedVectorWithUnitsTests, MultiplyTypedVectorOfLengths)
{
    TypedVector<Length, 3> a(1_m, 2_m, 3_m);
    TypedVector<Length, 3> b(1_m, 2_m, 3_m);
    TypedVector<Area, 3> area = a.Cross(b);
}

TEST(TypedTypedVectorWithUnitsTests, AddTypedVectorOfLengths)
{
    TypedVector<Length, 3> a(1_m, 2_m, 3_m);
    TypedVector<Quantity<LengthDimension>, 3> b(1_m, 2_m, 3_m);
    TypedVector<Length, 3> sum = a + b;
    EXPECT_EQ(sum.GetX(), 2_m);
}

TEST(TypedTypedVectorWithUnitsTests, GetLength)
{
    TypedVector<Length, 2> a(3_m, 4_m);
    EXPECT_EQ(a.Length(), 5_m);
}

TEST(TypedTypedVectorWithUnitsTests, Normalize)
{
    TypedVector<Length, 2> a(3_m, 4_m);
    TypedVector<double, 2> b = a.Normalize();
    EXPECT_DOUBLE_EQ(b.Length(), 1);
}
