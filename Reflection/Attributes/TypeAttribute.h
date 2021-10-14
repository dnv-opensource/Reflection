#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Type attribute. The storage type for a particluar type.
    */

    class TypeAttribute 
        : public IAttribute
    {
    public:
        TypeAttribute(const Reflection::Types::DecoratedTypeInfo& decoratedTypeInfo)
            : m_decoratedTypeInfo(decoratedTypeInfo)
        {}
        const Types::DecoratedTypeInfo GetDecoratedTypeInfo() const { return m_decoratedTypeInfo; }
        void SetDecoratedTypeInfo(const Types::DecoratedTypeInfo& val) { m_decoratedTypeInfo = val; }
    private:
        Reflection::Types::DecoratedTypeInfo m_decoratedTypeInfo;
    };
}}}}
