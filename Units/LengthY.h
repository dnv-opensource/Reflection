#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Length.h"
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Services { namespace Formatting {
    class FormattingService;
}}}}

namespace DNVS {namespace MoFa {namespace Units {
    class LengthY : public Length {
    public:
        LengthY(const LengthY& other) = default;
        LengthY(const Length& other) : Length(other) {}
        LengthY(const Quantity<LengthDimension>& other) : Length(other) {}
        LengthY(const QuantityBase<LengthDimension>& other) : Length(other) {}
        LengthY(const NamedQuantity<LengthDimension>& other) : Length(other) {}
        explicit LengthY(double value) : Length(value) {}
        LengthY() {}
        LengthY& operator=(const LengthY& other) = default;
        LengthY& operator=(const Length& other) { Length::operator=(other); return *this; }
        LengthY& operator=(const Quantity<LengthDimension>& other) { Length::operator=(other); return *this; }
    };
    std::string UNITS_IMPORT_EXPORT ToString(const LengthY& length, const DNVS::MoFa::Formatting::FormattingService& service);
}}}
