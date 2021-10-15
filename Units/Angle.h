#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include "NamedQuantity.h"
#include "IPhenomenon.h"

#define _USE_MATH_DEFINES
#include <math.h>

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 0, 0, 0, 1> AngleDimension;
    class AnglePhenomenon : public IPhenomenon
    {
    public:
        typedef AngleDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(AngleDimension());
        }
        static std::string GetName() { return "Angle"; }
    };


    typedef Quantity<AnglePhenomenon> Angle;
    typedef NamedQuantity<AngleDimension> NamedAngle;

    template<>
    class Quantity<AngleDimension> : public QuantityBase<AngleDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<AngleDimension>& other) : BaseClass(other) {}
        Quantity(const NamedAngle& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }
       
        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);

        friend double sin(Angle a) {return ::sin(a.GetValue()); }
        friend double cos(Angle a) {return ::cos(a.GetValue()); }
        friend double tan(Angle a) {return ::tan(a.GetValue()); }
    };

    const static NamedAngle _rad(1, "rad");
    inline Angle operator""_rad(long double value) { return value * _rad; }
    inline Angle operator""_rad(unsigned long long value) { return (double)value * _rad; }

    const static NamedAngle _deg(M_PI / 180.0, "deg");
    inline Angle operator""_deg(long double value) { return value * _deg; }
    inline Angle operator""_deg(unsigned long long value) { return (double)value * _deg; }

    const static NamedAngle _rev(2 * M_PI, "rev");
    inline Angle operator""_rev(long double value) { return value * _rev; }
    inline Angle operator""_rev(unsigned long long value) { return (double)value * _rev; }

    template<typename CallbackT>
    static void Quantity<AngleDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_rad);
        callback(_deg);
        callback(_rev);
    }

}}}
