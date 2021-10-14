#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting/IFormatter.h"
#include <string>

namespace DNVS {namespace MoFa {namespace Formatting {
    class FormattingService;
}}}
namespace DNVS {namespace MoFa {namespace Units { 
    class LengthX;
    class LengthY;
    class LengthZ;
}}}
namespace DNVS {namespace MoFa {namespace Units { namespace Formatting {
    class IDirectionalLengthFormatterService : public DNVS::MoFa::Formatting::IFormatter
    {
    public:
        virtual std::string Format(const LengthX& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const = 0;
        virtual std::string Format(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const = 0;
        virtual std::string Format(const LengthZ& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const = 0;
    };
}}}}