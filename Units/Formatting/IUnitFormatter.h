#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting\IFormatter.h"
#include <string>

namespace DNVS { namespace MoFa { namespace Units { namespace Runtime {
    class DynamicPhenomenon;
    class DynamicDimension;
}}}}

namespace DNVS { namespace MoFa { namespace Units { namespace Formatting {
    class IUnitFormatter : public DNVS::MoFa::Formatting::IFormatter
    {
    public:
        virtual std::string FormatQuantity(double number, const Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Formatting::FormattingService& formattingService) const = 0;
        virtual std::string FormatQuantity(double number, const Runtime::DynamicDimension& dimension, const DNVS::MoFa::Formatting::FormattingService& formattingService) const = 0;
    };
}}}}
