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
    typedef Dimension<1, 0, 0, 0, 0> LengthDimension;
    class LengthPhenomenon : public IPhenomenon
    {
    public:
        typedef LengthDimension DimensionType;
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(LengthDimension());
        }
        static std::string GetName() {return "Length";}
    };

    typedef Quantity<LengthPhenomenon> Length;
    typedef NamedQuantity<LengthDimension> NamedLength;

    template<>
    class Quantity<LengthDimension> : public QuantityBase<LengthDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedLength& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };

    const static NamedLength _km(1000, "km");
    inline Length operator""_km(long double value) { return value * _km; }
    inline Length operator""_km(unsigned long long value) { return (double)value * _km; }

    const static NamedLength _m(1, "m");
    inline Length operator""_m(long double value) { return value * _m; }
    inline Length operator""_m(unsigned long long value) { return (double)value * _m; }

    const static NamedLength _dm(0.1, "dm");
    inline Length operator""_dm(long double value) { return value * _dm; }
    inline Length operator""_dm(unsigned long long value) { return (double)value * _dm; }

    const static NamedLength _cm(0.01, "cm");
    inline Length operator""_cm(long double value) { return value * _cm; }
    inline Length operator""_cm(unsigned long long value) { return (double)value * _cm; }

    const static NamedLength _mm(0.001, "mm");
    inline Length operator""_mm(long double value) { return value * _mm; }
    inline Length operator""_mm(unsigned long long value) { return (double)value * _mm; }

    const static NamedLength _in(0.0254, "in");
    inline Length operator""_in(long double value) { return value * _in; }
    inline Length operator""_in(unsigned long long value) { return (double)value * _in; }

    const static NamedLength _ft(0.3048, "ft");
    inline Length operator""_ft(long double value) { return value * _ft; }
    inline Length operator""_ft(unsigned long long value) { return (double)value * _ft; }

    template<typename CallbackT>
    static void Quantity<LengthDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_m);
        callback(_dm);
        callback(_cm);
        callback(_mm);
        callback(_in);
        callback(_ft);
    }

}}}
