#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    struct CompareCaseLess {
        bool operator()(const std::string& lhs, const std::string& rhs) const
        {
            return _stricmp(lhs.c_str(), rhs.c_str()) < 0;
        }
    };
}}}