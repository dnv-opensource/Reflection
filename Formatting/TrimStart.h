#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IStringFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT TrimStart : public IStringFormatter
    {
    public:
        TrimStart(size_t numCharacters);
        std::string Format(const std::string& text) const override;
        virtual IStringFormatter* Clone() const;
    private:
        size_t m_numCharacters;
    };
}}}