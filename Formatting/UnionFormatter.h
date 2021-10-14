#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "IStringFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT UnionFormatter : public IStringFormatter
    {
    public:
        UnionFormatter(const IStringFormatter& lhs, const IStringFormatter& rhs);
        UnionFormatter(const UnionFormatter& other);
        ~UnionFormatter();
        virtual std::string Format(const std::string& text) const override;
        virtual IStringFormatter* Clone() const override;
    private:
        std::unique_ptr<IStringFormatter> m_lhs, m_rhs;
    };
    inline UnionFormatter operator+(const IStringFormatter& lhs, const IStringFormatter& rhs)
    {
        return UnionFormatter(lhs, rhs);
    }
}}}

