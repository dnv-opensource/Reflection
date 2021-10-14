#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Config.h"

namespace DNVS { namespace MoFa { namespace Formatting {
    class FORMATTING_IMPORT_EXPORT IStringFormatter {
    public:
        virtual ~IStringFormatter() {}
        virtual std::string Format(const std::string& text) const = 0;
        virtual IStringFormatter* Clone() const = 0;
    };

}}}
