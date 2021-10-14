//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReplaceSourceText.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    ReplaceSourceText::ReplaceSourceText(const std::string& replacementText) : m_replacementText(replacementText)
    {

    }

    std::string ReplaceSourceText::Format(const std::string& text) const
    {

        return m_replacementText;
    }

    DNVS::MoFa::Formatting::ReplaceSourceText* ReplaceSourceText::Clone() const
    {
        return new ReplaceSourceText(m_replacementText);
    }

}}}



