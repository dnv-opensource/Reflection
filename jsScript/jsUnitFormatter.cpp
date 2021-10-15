//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsUnitFormatter.h"
#include "Units\Runtime\DynamicPhenomenon.h"
#include "jsQuantity.h"

jsUnitFormatter::jsUnitFormatter(bool includeUnitString /*= true*/, bool getPrimitiveFormatterFromPhenomenon /*= false*/)
    : m_includeUnitString(includeUnitString)
    , m_getPrimitiveFormatterFromPhenomenon(getPrimitiveFormatterFromPhenomenon)
{
}

std::string jsUnitFormatter::FormatQuantity(double number, const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
{
    if (!phenomenon.IsValid())
        return "N/A";
    jsQuantity quantity(phenomenon, number, true);
    if (m_getPrimitiveFormatterFromPhenomenon)
        return jsQuantity(quantity).format(m_includeUnitString ? jsQuantity::includeUnitString : jsQuantity::excludeUnitString);
    else
        return std::string(jsQuantity(quantity));
}
