#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IEnumFormatter.h"
#include "Config.h"
#include <memory>
#include <list>

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT UnionEnumFormatter : public IEnumFormatter
    {
    public:
        UnionEnumFormatter(const std::shared_ptr<IEnumFormatter>& lhs, const std::shared_ptr<IEnumFormatter>& rhs);
        virtual std::string FormatEnum(int enumValue, const std::type_info& enumType, const FormattingService& service) const override;
        virtual bool IsSupported(const std::type_info&) const override;
    private:
        std::list<std::shared_ptr<IEnumFormatter>> m_formatters;
    };
}}}
