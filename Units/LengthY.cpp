//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "LengthY.h"
#include "Formatting/DefaultDirectionalLengthFormatterService.h"
#include "Formatting/FormattingService.h"

namespace DNVS {namespace MoFa {namespace Units {

    std::string ToString(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& service)
    {
        auto formatter = service.GetFormatterOrDefault<Formatting::IDirectionalLengthFormatterService, Formatting::DefaultDirectionalLengthFormatterService>();
        return formatter->Format(length, service);
    }

}}}

