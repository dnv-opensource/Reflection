//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstantLayoutSourceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    ConstantLayoutSourceProvider::ConstantLayoutSourceProvider(const std::shared_ptr<ILayoutSource>& layoutSource)
        : m_layoutSource(layoutSource)
    {

    }

    std::shared_ptr<Layout::ILayoutSource> ConstantLayoutSourceProvider::LookupLayoutSource(const std::string& name)
    {
        return m_layoutSource;
    }

}}}}

