#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class ILayoutSource;
    class ILayoutSourceProvider
    {
    public:
        virtual ~ILayoutSourceProvider() {}
        virtual std::shared_ptr<ILayoutSource> LookupLayoutSource(const std::string& name) = 0;
    };
}}}}