#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnitSystem.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    UnitSystem UNITS_IMPORT_EXPORT GetOutputUnitSystem();
    UnitSystem UNITS_IMPORT_EXPORT GetDatabaseUnitSystem();
    UnitSystem UNITS_IMPORT_EXPORT GetInputUnitSystem();

    template<typename QuantityT>
    QuantityT FromDatabase(double value)
    {
        return GetDatabaseUnitSystem().ConvertFromUnitSystem<QuantityT>(value);
    }

    template<typename QuantityT>
    QuantityT FromInput(double value)
    {
        return GetInputUnitSystem().ConvertFromUnitSystem<QuantityT>(value);
    }

}}}}