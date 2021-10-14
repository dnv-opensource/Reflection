//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultDirectionalLengthFormatterService.h"
#include "DefaultUnitFormatter.h"
#include "Formatting/FormattingService.h"
#include "../LengthX.h"
#include "../LengthY.h"
#include "../LengthZ.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Formatting {
    
    std::string DefaultDirectionalLengthFormatterService::Format(const LengthX& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        return formattingService.GetFormatterOrDefault<IUnitFormatter, DefaultUnitFormatter>()->FormatQuantity(length.GetValue(), LengthPhenomenon(), formattingService);
    }

    std::string DefaultDirectionalLengthFormatterService::Format(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        return formattingService.GetFormatterOrDefault<IUnitFormatter, DefaultUnitFormatter>()->FormatQuantity(length.GetValue(), LengthPhenomenon(), formattingService);
    }

    std::string DefaultDirectionalLengthFormatterService::Format(const LengthZ& length, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        return formattingService.GetFormatterOrDefault<IUnitFormatter, DefaultUnitFormatter>()->FormatQuantity(length.GetValue(), LengthPhenomenon(), formattingService);
    }

}}}}

