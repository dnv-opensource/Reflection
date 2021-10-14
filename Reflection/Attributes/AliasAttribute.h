#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "IAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class AliasAttribute : public IAttribute
    {
    public:
        template<typename T>
        AliasAttribute(const T& alias) : m_alias(Variants::VariantService::ReflectType<T>(alias)) {}
        Variants::Variant GetAlias() const { return m_alias; }
    private:
        Variants::Variant m_alias;
    };
}}}}
