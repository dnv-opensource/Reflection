//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "CombinedLayoutSourceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    CombinedLayoutSourceProvider::CombinedLayoutSourceProvider(const std::shared_ptr<ILayoutSourceProvider>& lhs, const std::shared_ptr<ILayoutSourceProvider>& rhs)
        : m_lhs(lhs)
        , m_rhs(rhs)
    {

    }

    std::shared_ptr<ILayoutSource> CombinedLayoutSourceProvider::LookupLayoutSource(const std::string& name)
    {
        auto source = m_lhs->LookupLayoutSource(name);
        if (!source)
            source = m_rhs->LookupLayoutSource(name);
        return source;
    }

}}}}


