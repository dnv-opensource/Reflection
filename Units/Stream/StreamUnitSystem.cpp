//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "StreamUnitSystem.h"
#include "StreamPointerManager.h"

namespace DNVS { namespace MoFa {namespace Units {namespace Stream {
    Runtime::UnitSystem* StreamUnitSystem::GetFromStream( std::ostream& stream )
    {
        return StreamPointerManager<StreamUnitSystem,Runtime::UnitSystem>::Get(stream);
    }

    void StreamUnitSystem::SetToStream( std::ostream& stream, Runtime::UnitSystem* system )
    {
        return StreamPointerManager<StreamUnitSystem,Runtime::UnitSystem>::Set(stream, system);
    }

    int StreamUnitSystem::GetPointerIndex1()
    {
        static int i = std::ios_base::xalloc();
        return i;
    }

    int StreamUnitSystem::GetPointerIndex2()
    {
        static int i = std::ios_base::xalloc();
        return i;
    }

}}}}