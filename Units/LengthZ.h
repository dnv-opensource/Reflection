#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Length.h"
#include "Config.h"

namespace DNVS {namespace MoFa {namespace Units {
    class LengthZ : public Length {
    public:
        LengthZ(const LengthZ& other) = default;
        LengthZ(const Length& other) : Length(other) {}
        LengthZ(const Quantity<LengthDimension>& other) : Length(other) {}
        LengthZ(const NamedQuantity<LengthDimension>& other) : Length(other) {}
        explicit LengthZ(double value) : Length(value) {}
        LengthZ() {}
        LengthZ& operator=(const LengthZ& other) = default;
        LengthZ& operator=(const Length& other) { Length::operator=(other); return *this; }
        LengthZ& operator=(const Quantity<LengthDimension>& other) { Length::operator=(other); return *this; }
    };

    std::string UNITS_IMPORT_EXPORT ToString(const LengthZ& length, const DNVS::MoFa::Formatting::FormattingService& service);
}}}
