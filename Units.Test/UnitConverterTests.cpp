//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Units\Length.h"
#include "Units\Runtime\UnitConverter.h"
#include "Units\Force.h"
#include "Units\Time.h"
#include "Units\TempDiff.h"
#include "Units\Runtime\UnitHasInvalidDimension.h"
#include "Units\Angle.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Test {
    TEST(UnitConverterTests, ConvertFromToSame)
    {
        UnitConverter converter(UnitSystem(_m, _N, _delC, _s), UnitSystem(_m, _N, _delC, _s));
        EXPECT_DOUBLE_EQ(1,converter.Convert<Length>(1));
    }

    TEST(UnitConverterTests, ThrowOnIncorrectUnits)
    {
        EXPECT_THROW(UnitSystem(_m, _s, _delC), UnitHasInvalidDimension);
    }
    
    TEST(UnitConverterTests, ConvertFrom_meter_to_millimeter)
    {
        UnitConverter converter(UnitSystem(_m, _N), UnitSystem(_mm, _N));
        EXPECT_DOUBLE_EQ(1000,converter.Convert<Length>(1));
    }

    TEST(UnitConverterTests, ChangeToSystem)
    {
        UnitConverter converter(UnitSystem(_m, _N), UnitSystem(_m, _N));
        EXPECT_DOUBLE_EQ(1,converter.Convert<Length>(1));
        converter.SetToSystem(UnitSystem(_mm, _N));
        EXPECT_DOUBLE_EQ(1000,converter.Convert<Length>(1));
    }

    TEST(UnitConverterTests, ConvertAngle_Simple)
    {
        Angle a = 1_deg;
        EXPECT_DOUBLE_EQ(1, a.ConvertTo(_deg));
    }
    TEST(UnitConverterTests, ConvertLength_Simple)
    {
        Length a = 2.88_in;
        EXPECT_DOUBLE_EQ(2.88, a.ConvertTo(_in));
    }
}}}}}