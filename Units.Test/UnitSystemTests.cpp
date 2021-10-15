//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "Units\Runtime\UnitSystem.h"
#include "Units\Length.h"
#include "Units\Angle.h"
#include "Units\TempDiff.h"
#include "Units\Force.h"
#include "Units\Time.h"
#include "Units\Mass.h"
#include "Units\Energy.h"
#include "Units\MomentOfForce.h"
#include "Units\StreamSupport.h"
#include "Units\Runtime\DynamicQuantity.h"
#include "Units\Acceleration.h"
#include "GoogleTest\Profiler.h"
#include "Units\Runtime\DatabaseUnitSystem.h"
#include "Services\ScopedServiceProvider.h"
#include "Units\Runtime\DefaultUnitProvider.h"
#include "Units\Runtime\DefaultOutputUnitProvider.h"
#include "Units\Runtime\DefaultInputUnitProvider.h"
#include "Units\Area.h"
#include "Units\MassPerUnitLength.h"
#include "Units\SecondMomentOfArea.h"
#include "Units\FirstMomentOfArea.h"
#include "Units\Runtime\InputUnitHelper.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Tests {
    TEST(UnitSystemTests, CreateDefaultUnitSystem_GetBaseUnits)
    {
        UnitSystem system;
        ASSERT_EQ(_m, system.GetUnit(LengthDimension()));
        ASSERT_EQ(_s, system.GetUnit(TimeDimension()));
        ASSERT_EQ(_N, system.GetUnit(ForceDimension()));
        ASSERT_EQ(_delC, system.GetUnit(TempDiffDimension()));
        ASSERT_EQ(_rad, system.GetUnit(AngleDimension()));
    }

    TEST(UnitSystemTests, CreateDefaultUnitSystem_GetDerivedUnits)
    {
        UnitSystem system;
        ASSERT_EQ(_kg, system.GetUnit(MassDimension()));
        ASSERT_EQ("kg", system.GetUnit(MassDimension()).GetUnitName());
    }

    TEST(UnitSystemTests, CreateDefaultUnitSystem_GetEnergyUnit)
    {
        UnitSystem system;
        ASSERT_EQ("J", system.GetUnit(EnergyDimension()).GetUnitName());
        ASSERT_EQ("J", system.GetUnit(EnergyPhenomenon()).GetUnitName());
    }

    TEST(UnitSystemTests, CreateDefaultUnitSystem_GetMomentOfForcePhenomenonUnit)
    {
        UnitSystem system;
        ASSERT_EQ("N*m", system.GetUnit(MomentOfForcePhenomenon()).GetUnitName());
        ASSERT_EQ("J", system.GetUnit(MomentOfForceDimension()).GetUnitName());
    }

    TEST(UnitSystemTests, CreateImperialUnitSystem_GetEnergyUnit)
    {
        UnitSystem system(_ft,_lbf);
        ASSERT_EQ("ft*lbf", system.GetUnit(EnergyDimension()).GetUnitName());
    }

    TEST(UnitSystemTests, StreamUsingCustomUnitSystem)
    {
        UnitSystem system(_ft, _lbf);
        std::stringstream stream;
        Stream::StreamUnitSystem::SetToStream(stream, &system);
        Stream::ShowUnitString::SetToStream(stream, true);
        stream << Length(100);
        ASSERT_EQ("328.084 ft", stream.str());
    }

    TEST(UnitSystemTests, CreateDynamicQuantityFromAcceleration)
    {
        UnitSystem system(_ft, _lbf);
        DynamicQuantity qty(Acceleration(1),system);
        EXPECT_EQ("ft/s^2",qty.GetUnit().GetUnitName());
        EXPECT_DOUBLE_EQ(_m/_ft,qty.GetValue());
    }    

    TEST(UnitSystemTests, TestSystemConvertTo)
    {
        UnitSystem system(_ft, _lbf);
        EXPECT_DOUBLE_EQ(_m/_ft,system.Convert(Acceleration(1)));
    }

    TEST(UnitSystemTests, TestConvertRuntimeQuantity)
    {
        UnitSystem system(_ft, _lbf);
        EXPECT_DOUBLE_EQ(_m/_ft,system.Convert(DynamicQuantity(1000, Unit(_mm)/Pow(Unit(_s),2))));
    }

    TEST(UnitSystemTests, GetDataBaseUnitSystemForEmptyModelReturnsSIUnitSystem)
    {
        EXPECT_DOUBLE_EQ(1,GetDatabaseUnitSystem().Convert(Length(1)));
    }

    TEST(UnitSystemTests, GetOutputUnitSystemWhenIUnitProviderRegisteredWithSingletonReturnsIUnitProvidersSystem)
    {
        Services::ScopedServiceProvider provider;
        Services::ServiceProvider::Instance().RegisterService<IOutputUnitProvider>(std::make_shared<DefaultOutputUnitProvider>(std::make_shared<DefaultUnitProvider>(_ft, _lbf)));
        EXPECT_DOUBLE_EQ(_m/_ft,GetOutputUnitSystem().Convert(Length(1)));
    }

    TEST(UnitSystemTests, PerformanceOfUnitConversionUsingUnitSystemIsSameOrderOfMagnitudeAsDirectConversion)
    {
        auto baseline = [&]() -> double
        {
            auto length = _m;
            auto length2 = _ft;
            auto acceleration = Acceleration(1);
            return (acceleration/length2*length).GetValue();
        };
        UnitSystem system(_ft, _lbf);
        auto contender = [&]() -> double
        {
            return system.Convert(Acceleration(1));
        };
        Profile(baseline, contender, 10);
    }

    TEST(UnitSystemTests, ConvertFromMilliMeterToCentimeter)
    {
        UnitSystem system(_mm, _N);
        EXPECT_DOUBLE_EQ(10,system.ConvertToUnit(100,_cm));
    }    

    TEST(UnitSystemTests, TestConvertFromDimensionToUnit_SimplifyCorrectly)
    {
        UnitSystem system;
        EXPECT_EQ("N/m", system.GetUnit(DynamicDimension(-1, 0, 1, 0, 0)).GetUnitName());
    }

    TEST(UnitSystemTests, TestConversionFromRadToDeg)
    {
        UnitSystem system("m","N","delC","s","deg");
        EXPECT_NEAR(180,system.Convert(3.14159265359_rad),1e-4);
    }

    void SetSIUnits(InputUnitHelper& units)
    {
        units.SetUnits<Length>(_mm);
        units.SetUnits<Area>(_mm2);
        units.SetUnits<MassPerUnitLength>(_kg / _m);
        units.SetUnits<SecondMomentOfArea>(_mm4 / 1e6);
        units.SetUnits<FirstMomentOfArea>(_mm3 / 1e3);
    }

    TEST(UnitSystemTests, TestCustomUnitSystem_ConvertUnits)
    {
        Services::ScopedServiceProvider provider;
        Services::ServiceProvider::Instance().RegisterService<IInputUnitProvider>(std::make_shared<DefaultInputUnitProvider>());
        InputUnitHelper units(Services::ServiceProvider::Instance().GetService<IInputUnitProvider>());
        SetSIUnits(units);
        EXPECT_EQ(1.2_mm2, Runtime::GetInputUnitSystem().ConvertFromUnitSystem<Area>(1.2));
        EXPECT_DOUBLE_EQ((1.2_mm4/1e6).GetValue(), Runtime::GetInputUnitSystem().ConvertFromUnitSystem<SecondMomentOfArea>(1.2).GetValue());
    }

}}}}}