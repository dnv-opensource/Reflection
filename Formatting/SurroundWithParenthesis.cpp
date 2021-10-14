//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SurroundWithParenthesis.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    std::string SurroundWithParenthesis::Format(const std::string& text) const
    {
        return "(" + text + ")";
    }

    IStringFormatter* SurroundWithParenthesis::Clone() const
    {
        return new SurroundWithParenthesis();
    }

}}}

