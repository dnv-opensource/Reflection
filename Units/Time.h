#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include "NamedQuantity.h"
#include "IPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 1, 0, 0, 0> TimeDimension;
    class TimePhenomenon : public IPhenomenon
    {
    public:
        typedef TimeDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(TimeDimension());
        }
        static std::string GetName() { return "Time"; }
    };

    typedef Quantity<TimePhenomenon> Time;

    typedef NamedQuantity<TimeDimension> NamedTime;

    template<>
    class Quantity<TimeDimension> : public QuantityBase<TimeDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedTime& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };

    const static NamedTime _s(1.0, "s");
    inline Time operator""_s(long double value) { return value * _s; }
    inline Time operator""_s(unsigned long long value) { return (double)value * _s; }

    const static Quantity<Dimension<0, 2, 0, 0, 0>> _s2(1.0);
    inline Quantity<Dimension<0, 2, 0, 0, 0>> operator""_s2(long double value) { return value * _s2; }
    inline Quantity<Dimension<0, 2, 0, 0, 0>> operator""_s2(unsigned long long value) { return (double)value * _s2; }

    const static NamedTime _ms(1e-3, "ms");
    inline Time operator""_ms(long double value) { return value * _ms; }
    inline Time operator""_ms(unsigned long long value) { return (double)value * _ms; }

    const static Quantity<Dimension<0, 2, 0, 0, 0>> _ms2(pow(1e-3, 2));
    inline Quantity<Dimension<0, 2, 0, 0, 0>> operator""_ms2(long double value) { return value * _ms2; }
    inline Quantity<Dimension<0, 2, 0, 0, 0>> operator""_ms2(unsigned long long value) { return (double)value * _ms2; }

    const static NamedTime _min(60.0, "min");
    inline Time operator""_min(long double value) { return value * _min; }
    inline Time operator""_min(unsigned long long value) { return (double)value * _min; }

    const static NamedTime _hr(3600.0, "hr");
    inline Time operator""_hr(long double value) { return value * _hr; }
    inline Time operator""_hr(unsigned long long value) { return (double)value * _hr; }

    template<typename CallbackT>
    static void Quantity<TimeDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_ms);
        callback(_s);
        callback(_min);
        callback(_hr);
    }

}}}