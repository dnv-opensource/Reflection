#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutSourceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ConstantLayoutSourceProvider : public ILayoutSourceProvider
    {
    public:
        ConstantLayoutSourceProvider(const std::shared_ptr<ILayoutSource>& layoutSource);
        virtual std::shared_ptr<ILayoutSource> LookupLayoutSource(const std::string& name) override;
    private:
        std::shared_ptr<ILayoutSource> m_layoutSource;
    };
}}}}