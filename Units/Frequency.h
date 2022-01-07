#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "NamedQuantity.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "IPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, -1, 0, 0, 0> FrequencyDimension;
    class FrequencyPhenomenon : public IPhenomenon
    {
    public:
        typedef FrequencyDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(FrequencyDimension());
        }
        static std::string GetName() { return "Frequency"; }
    };

    typedef Quantity<FrequencyPhenomenon> Frequency;
    typedef NamedQuantity<FrequencyDimension> NamedFrequency;

    template<>
    class Quantity<FrequencyDimension> : public QuantityBase<FrequencyDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedFrequency& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    const static NamedFrequency _Hz(1, "Hz");
    inline Frequency operator""_Hz(long double value) { return value * _Hz; }
    inline Frequency operator""_Hz(unsigned long long value) { return (double)value * _Hz; }

    const static NamedFrequency _MHz(1e6, "MHz");
    inline Frequency operator""_MHz(long double value) { return value * _MHz; }
    inline Frequency operator""_MHz(unsigned long long value) { return (double)value * _MHz; }

    const static NamedFrequency _radps(2 * M_PI, "radps");
    inline Frequency operator""_radps(long double value) { return value * _radps; }
    inline Frequency operator""_radps(unsigned long long value) { return (double)value * _radps; }

    template<typename CallbackT>
    static void Quantity<FrequencyDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_Hz);
        callback(_MHz);
        callback(_radps);
    }

}}}

