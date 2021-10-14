//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DatabaseUnitSystem.h"
#include "Services\ServiceProvider.h"
#include "IUnitProvider.h"
#include "IInputUnitProvider.h"
#include "IOutputUnitProvider.h"
#include "IDatabaseUnitProvider.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    UnitSystem GetOutputUnitSystem()
    {
        auto unitProvider = Services::ServiceProvider::Instance().TryGetService<IOutputUnitProvider>();
        if (unitProvider)
            return UnitSystem(unitProvider->GetUnitProvider());
        else
            return UnitSystem();
    }

    UnitSystem GetDatabaseUnitSystem()
    {
        auto unitProvider = Services::ServiceProvider::Instance().TryGetService<IDatabaseUnitProvider>();
        if (unitProvider)
            return UnitSystem(unitProvider->GetUnitProvider());
        else
            return UnitSystem();
    }

    UnitSystem GetInputUnitSystem()
    {
        auto unitProvider = Services::ServiceProvider::Instance().TryGetService<IInputUnitProvider>();
        if (unitProvider)
            return UnitSystem(unitProvider->GetUnitProvider());
        else
            return UnitSystem();
    }

}}}}