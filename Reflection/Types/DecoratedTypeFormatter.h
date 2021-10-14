#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DecoratedTypeInfo.h"
#include "ITypeFormatter.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    class DecoratedTypeFormatter : public ITypeFormatter 
    {
    public:
        DecoratedTypeFormatter(std::shared_ptr<ITypeFormatter> typeFormatter) : m_typeFormatter(typeFormatter) {}
        virtual std::string FormatType(const DecoratedTypeInfo& typeInfo) const 
        {
            std::string text = m_typeFormatter->FormatType(typeInfo);
            std::string decoration = GetDecorationName(typeInfo.GetDecoration());
            return text + decoration;
        }
    private:
        std::shared_ptr<ITypeFormatter> m_typeFormatter;
    };
}}}}