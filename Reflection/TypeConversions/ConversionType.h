#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    struct ConversionType {
        enum Type {
            NoConversion,
            TrivialConversion,      ///conversion from T& to const T&, T* to const T*, T to const T&, const T& to T etc.
            PromotionConversion,    ///float to double, short to int etc. Non-lossy conversion.
            StandardConversion,     ///int to float, double to float, unsigned int to signed int etc. (Potentially) Lossy conversion.
            UpcastConversion,       ///Up casting from derived to base
            UserConversion,         ///Any custom conversion, such as const char* to std::string (constructor conversion) or mfNumberOrEnum<DaysEnum> to DaysEnum (conversion operator)
            DowncastConversion,     ///Down casting from base to derived
            DynamicTypeConversion,  ///Getting the most concrete type out of a pointer.
            LValueConversion,       ///Creates an lvalue based on the input type.
            FallbackConversion,     ///A conversion from a runtime type to a compile time type. Example: from RuntimeQuantity(5,Unit("m/s^2")) to Acceleration.
            InfiniteConversion,     ///No conversion found
            ReflectionConversion,   ///Converts to Variant or to Object
        };
    };

}}}}

