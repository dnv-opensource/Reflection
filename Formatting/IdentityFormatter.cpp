//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IdentityFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    IdentityFormatter::IdentityFormatter()
    {

    }

    std::string IdentityFormatter::Format(const std::string& text) const
    {
        return text;
    }

    IdentityFormatter* IdentityFormatter::Clone() const
    {
        return new IdentityFormatter;
    }

    const IdentityFormatter& IdentityFormatter::GetIdentityFormatter()
    {
        return s_formatter;
    }

    IdentityFormatter IdentityFormatter::s_formatter;

}}}


