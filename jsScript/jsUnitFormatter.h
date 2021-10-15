#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units/Formatting/IUnitFormatter.h"
#include "jsScript_config.h"

#pragma warning(push)
#pragma warning(disable:4275)
class JSSCRIPT_IMPORT_EXPORT jsUnitFormatter : public DNVS::MoFa::Units::Formatting::IUnitFormatter
{
public:
    jsUnitFormatter(bool includeUnitString = true, bool getPrimitiveFormatterFromPhenomenon = false);
    virtual std::string FormatQuantity(double number, const DNVS::MoFa::Units::Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
private:
    //Includes the unit string in the returned text, "45 mm" instead of "45".
    bool m_includeUnitString;
    //If true, we will use the formatting rules from the phenomenon when formatting the number, so that length and mass may be formatted differently, for example.
    bool m_getPrimitiveFormatterFromPhenomenon;
};
#pragma warning(pop)