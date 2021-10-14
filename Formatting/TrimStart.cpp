//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TrimStart.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    TrimStart::TrimStart(size_t numCharacters) : m_numCharacters(numCharacters)
    {

    }

    std::string TrimStart::Format(const std::string& text) const
    {
        return text.substr(m_numCharacters);
    }

    IStringFormatter* TrimStart::Clone() const
    {
        return new TrimStart(m_numCharacters);
    }

}}}

