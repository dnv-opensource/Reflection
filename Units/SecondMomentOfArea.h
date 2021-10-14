#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<4, 0, 0, 0, 0> SecondMomentOfAreaDimension;
    class SecondMomentOfAreaPhenomenon : public IPhenomenon
    {
    public:
        typedef SecondMomentOfAreaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()), 4);
        }
        static std::string GetName() { return "SecondMomentOfArea"; }
    };

    typedef NamedQuantity<SecondMomentOfAreaDimension> NamedSecondMomentOfArea;
    typedef Quantity<SecondMomentOfAreaPhenomenon> SecondMomentOfArea;

    const static NamedSecondMomentOfArea _km4(pow(1000, 4), "km^4");
    inline SecondMomentOfArea operator""_km4(long double value) { return value * _km4; }
    inline SecondMomentOfArea operator""_km4(unsigned long long value) { return (double)value * _km4; }

    const static NamedSecondMomentOfArea _m4(1, "m^4");
    inline SecondMomentOfArea operator""_m4(long double value) { return value * _m4; }
    inline SecondMomentOfArea operator""_m4(unsigned long long value) { return (double)value * _m4; }

    const static NamedSecondMomentOfArea _dm4(pow(0.1, 4), "dm^4");
    inline SecondMomentOfArea operator""_dm4(long double value) { return value * _dm4; }
    inline SecondMomentOfArea operator""_dm4(unsigned long long value) { return (double)value * _dm4; }

    const static NamedSecondMomentOfArea _cm4(pow(0.01, 4), "cm^4");
    inline SecondMomentOfArea operator""_cm4(long double value) { return value * _cm4; }
    inline SecondMomentOfArea operator""_cm4(unsigned long long value) { return (double)value * _cm4; }

    const static NamedSecondMomentOfArea _mm4(pow(0.001, 4), "mm^4");
    inline SecondMomentOfArea operator""_mm4(long double value) { return value * _mm4; }
    inline SecondMomentOfArea operator""_mm4(unsigned long long value) { return (double)value * _mm4; }

    const static NamedSecondMomentOfArea _in4(pow(0.0254, 4), "in^4");
    inline SecondMomentOfArea operator""_in4(long double value) { return value * _in4; }
    inline SecondMomentOfArea operator""_in4(unsigned long long value) { return (double)value * _in4; }

    const static NamedSecondMomentOfArea _ft4(pow(0.3048, 4), "ft^4");
    inline SecondMomentOfArea operator""_ft4(long double value) { return value * _ft4; }
    inline SecondMomentOfArea operator""_ft4(unsigned long long value) { return (double)value * _ft4; }

}}}


