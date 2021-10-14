#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/VariantService.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class NamedItem
    {
    public:
        template<typename T>
        NamedItem(T value, const std::string& name)
            : m_name(name)
            , m_variant(Variants::VariantService::ReflectType<T>(value))
        {}
        NamedItem(const Variants::Variant& value, const std::string& name)
            : m_name(name)
            , m_variant(value)
        {}
        const std::string& GetName() const { return m_name; }
        const Variants::Variant& GetVariant() const { return m_variant; }
    private:
        std::string m_name;
        Variants::Variant m_variant;
    };
}}}}
