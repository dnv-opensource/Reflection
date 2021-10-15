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
    class LengthX : public Length {
    public:
        LengthX(const LengthX& other) = default;
        LengthX(const Length& other) : Length(other) {}
        LengthX(const Quantity<LengthDimension>& other) : Length(other) {}
        LengthX(const QuantityBase<LengthDimension>& other) : Length(other) {}
        LengthX(const NamedQuantity<LengthDimension>& other) : Length(other) {}
        explicit LengthX(double value) : Length(value) {}
        LengthX() {}
        LengthX& operator=(const LengthX& other) = default;
        LengthX& operator=(const Length& other) { Length::operator=(other); return *this; }
        LengthX& operator=(const Quantity<LengthDimension>& other) { Length::operator=(other); return *this; }
    };
    
    std::string UNITS_IMPORT_EXPORT ToString(const LengthX& length, const DNVS::MoFa::Formatting::FormattingService& service);
}}}
