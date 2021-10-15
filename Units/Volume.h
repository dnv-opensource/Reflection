#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include "NamedQuantity.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<3, 0, 0, 0, 0> VolumeDimension;
    class VolumePhenomenon : public IPhenomenon
    {
    public:
        typedef VolumeDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()),3);
        }
        static std::string GetName() { return "Volume"; }
    };

    typedef Quantity<VolumePhenomenon> Volume;
    typedef NamedQuantity<VolumeDimension> NamedVolume;

    template<>
    class Quantity<VolumeDimension> : public QuantityBase<VolumeDimension>
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<VolumeDimension>& other) : BaseClass(other) {}
        Quantity(const NamedVolume& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    const static NamedVolume _l(0.001, "l");
    inline Volume operator""_l(long double value) { return value * _l; }
    inline Volume operator""_l(unsigned long long value) { return (double)value * _l; }

    const static NamedVolume _dl(0.0001, "dl");
    inline Volume operator""_dl(long double value) { return value * _dl; }
    inline Volume operator""_dl(unsigned long long value) { return (double)value * _dl; }

    const static NamedVolume _cl(0.00001, "cl");
    inline Volume operator""_cl(long double value) { return value * _dl; }
    inline Volume operator""_cl(unsigned long long value) { return (double)value * _dl; }

    const static NamedVolume _ml(0.000001, "ml");
    inline Volume operator""_ml(long double value) { return value * _cl; }
    inline Volume operator""_ml(unsigned long long value) { return (double)value * _cl; }

    const static NamedVolume _km3(pow(1000, 3), "km^3");
    inline Volume operator""_km3(long double value) { return value * _km3; }
    inline Volume operator""_km3(unsigned long long value) { return (double)value * _km3; }

    const static NamedVolume _m3(1, "m^3");
    inline Volume operator""_m3(long double value) { return value * _m3; }
    inline Volume operator""_m3(unsigned long long value) { return (double)value * _m3; }

    const static NamedVolume _dm3(pow(0.1, 3), "dm^3");
    inline Volume operator""_dm3(long double value) { return value * _dm3; }
    inline Volume operator""_dm3(unsigned long long value) { return (double)value * _dm3; }

    const static NamedVolume _cm3(pow(0.01, 3), "cm^3");
    inline Volume operator""_cm3(long double value) { return value * _cm3; }
    inline Volume operator""_cm3(unsigned long long value) { return (double)value * _cm3; }

    const static NamedVolume _mm3(pow(0.001, 3), "mm^3");
    inline Volume operator""_mm3(long double value) { return value * _mm3; }
    inline Volume operator""_mm3(unsigned long long value) { return (double)value * _mm3; }

    const static NamedVolume _in3(pow(0.0254, 3), "in^3");
    inline Volume operator""_in3(long double value) { return value * _in3; }
    inline Volume operator""_in3(unsigned long long value) { return (double)value * _in3; }

    const static NamedVolume _ft3(pow(0.3048, 3), "ft^3");
    inline Volume operator""_ft3(long double value) { return value * _ft3; }
    inline Volume operator""_ft3(unsigned long long value) { return (double)value * _ft3; }

    template<typename CallbackT>
    static void Quantity<VolumeDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_l);
        callback(_dl);
        callback(_cl);
        callback(_ml);
    }
}}}
