#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutSourceProvider.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class CombinedLayoutSourceProvider
        : public ILayoutSourceProvider
    {
    public:
        CombinedLayoutSourceProvider(const std::shared_ptr<ILayoutSourceProvider>& lhs, const std::shared_ptr<ILayoutSourceProvider>& rhs);
        virtual std::shared_ptr<ILayoutSource> LookupLayoutSource(const std::string& name) override;
    private:
        std::shared_ptr<ILayoutSourceProvider> m_lhs;
        std::shared_ptr<ILayoutSourceProvider> m_rhs;
    };
}}}}
