//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Services\ScopedServiceProvider.h"
#include "gtest\gtest.h"
#include "Services\ServiceProvider.h"
#include "Units\Runtime\DefaultInputUnitProvider.h"
#include "Units\Runtime\DatabaseUnitSystem.h"
#include "Units\TempDiff.h"
#include "Units\Force.h"
#include "Units\Length.h"
#include "Units\Runtime\TemporaryUnitSystem.h"

namespace DNVS { namespace MoFa {namespace Units {namespace Runtime {namespace Tests {
    TEST(TemporaryInputUnitSystemTests, CreateUnitSystem_TemporarySystem_DonotTouchOriginal)
    {
        Services::ScopedServiceProvider provider;
        Services::ServiceProvider::Instance().RegisterService<IInputUnitProvider>(std::make_shared<DefaultInputUnitProvider>());
        GetInputUnitSystem().SetUnit<Length>("ft");
        GetInputUnitSystem().SetUnit<Force>("kipf");
        GetInputUnitSystem().SetUnit<TempDiff>("delF");
        {
            TemporaryInputUnitSystem tempUnitSystem(TemporaryInputUnitSystem::CopyAllUnits);
            GetInputUnitSystem().SetUnit<Length>("m");
            GetInputUnitSystem().SetUnit<Force>("kN");
            GetInputUnitSystem().SetUnit<TempDiff>("delC");
        }
        EXPECT_EQ("ft", GetInputUnitSystem().GetLengthUnitString());
        EXPECT_EQ("kipf", GetInputUnitSystem().GetForceUnitString());
        EXPECT_EQ("delF", GetInputUnitSystem().GetTempDiffUnitString());
    }
} } } } }