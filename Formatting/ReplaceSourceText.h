#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IStringFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT ReplaceSourceText : public IStringFormatter
    {
    public:
        ReplaceSourceText(const std::string& replacementText);
        std::string Format(const std::string& text) const override;
        virtual ReplaceSourceText* Clone() const override;
    private:
        std::string m_replacementText;
    };
}}}

