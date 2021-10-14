//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "EmptyUnit.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime { namespace Detail {

    const std::shared_ptr<IUnit>& EmptyUnit::GetEmptyUnit()
    {
        if (!m_emptyUnit)
            m_emptyUnit = std::make_shared<EmptyUnit>();
        return m_emptyUnit;
    }

    std::shared_ptr<IUnit> EmptyUnit::m_emptyUnit;
}}}}}
