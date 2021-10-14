#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ColumnWithHeader.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
    template<typename DerivedT>
    class GenericColumn : public ColumnWithHeader
    {
    public:
        GenericColumn(const std::string& header) : ColumnWithHeader(header) {}
        GenericColumn(const GenericColumn& other) = default;

        template<typename T>
        DerivedT AddDefault(const T& arg)
        {
            SetDefault(Variants::VariantService::ReflectType<T>(arg));
            return static_cast<DerivedT&>(*this);
        }
        DerivedT AddDocumentation(const std::string& documentation)
        {
            SetDocumentation(documentation);
            return static_cast<DerivedT&>(*this);
        }
    };
}}}}
