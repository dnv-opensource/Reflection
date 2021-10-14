#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units/Runtime/DynamicPhenomenon.h"
#include <iosfwd>
#include <memory>

namespace DNVS {namespace MoFa {namespace Formatting {
    class IPrimitiveFormatter;
}}}
namespace DNVS {namespace MoFa {namespace Units { namespace Formatting {

    /**
    This interface provides information about how units of different quantities should be formatted.
    */
    class IUnitFormatterService
    {
    public:
        virtual std::shared_ptr<DNVS::MoFa::Formatting::IPrimitiveFormatter> GetPrimitiveFormatter(const Runtime::DynamicPhenomenon& phenomenon) const = 0;
        virtual std::ios_base::fmtflags GetFloatFlags(const Runtime::DynamicPhenomenon& phenomenon) const = 0;
        virtual int GetPrecision(const Runtime::DynamicPhenomenon& phenomenon) const = 0;
    };
}}}}