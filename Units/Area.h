#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<2, 0, 0, 0, 0> AreaDimension;
    class AreaPhenomenon : public IPhenomenon
    {
    public:
        typedef AreaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()),2);
        }
        static std::string GetName() { return "Area"; }
    };
    typedef Quantity<AreaPhenomenon> Area;
    typedef NamedQuantity<AreaDimension> NamedArea;

    const static NamedArea _km2(1000 * 1000, "km^2");
    inline Area operator""_km2(long double value) { return value * _km2; }
    inline Area operator""_km2(unsigned long long value) { return (double)value * _km2; }

    const static NamedArea _m2(1, "m^2");
    inline Area operator""_m2(long double value) { return value * _m2; }
    inline Area operator""_m2(unsigned long long value) { return (double)value * _m2; }

    const static NamedArea _dm2(0.1*0.1, "dm^2");
    inline Area operator""_dm2(long double value) { return value * _dm2; }
    inline Area operator""_dm2(unsigned long long value) { return (double)value * _dm2; }

    const static NamedArea _cm2(0.010*0.01, "cm^2");
    inline Area operator""_cm2(long double value) { return value * _cm2; }
    inline Area operator""_cm2(unsigned long long value) { return (double)value * _cm2; }

    const static NamedArea _mm2(0.001*0.001, "mm^2");
    inline Area operator""_mm2(long double value) { return value * _mm2; }
    inline Area operator""_mm2(unsigned long long value) { return (double)value * _mm2; }

    const static NamedArea _in2(0.0254*0.0254, "in^2");
    inline Area operator""_in2(long double value) { return value * _in2; }
    inline Area operator""_in2(unsigned long long value) { return (double)value * _in2; }

    const static NamedArea _ft2(0.3048*0.3048, "ft^2");
    inline Area operator""_ft2(long double value) { return value * _ft2; }
    inline Area operator""_ft2(unsigned long long value) { return (double)value * _ft2; }

}}}


