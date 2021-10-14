#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    //This indicates that a member is not required to be filled in.
    //Also indicates if a class represents an optional value.
    class OptionalAttribute 
    {
    public:
        OptionalAttribute() {}
        OptionalAttribute(const Types::DecoratedTypeInfo& actualType) 
            : m_actualType(actualType)
        {}
        const Types::DecoratedTypeInfo& GetActualType() const { return m_actualType; }
    private:
        Types::DecoratedTypeInfo m_actualType;
    };
}}}}