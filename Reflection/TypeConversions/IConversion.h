#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/ConversionType.h"
#include "Reflection/Variants/Variant.h"
#include <memory>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    class IConversion {
    public:
        // creates copy of variable, converts it and returns converted copy
        virtual Variants::Variant Convert(const Variants::Variant& variable) = 0;

        // converts variable itself (it is overwritten)
        virtual void IntrusiveConvert(Variants::Variant& variable) = 0;
    };
    typedef std::shared_ptr<IConversion> ConversionPointer;
}}}}
