#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "NamedQuantity.h"
#include "Length.h"
#include "Time.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<1, -1, 0, 0, 0> VelocityDimension;
    class VelocityPhenomenon : public IPhenomenon
    {
    public:
        typedef VelocityDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(LengthDimension()) / callback(TimeDimension());
        }
        static std::string GetName() { return "Velocity"; }
    };

    typedef Quantity<VelocityPhenomenon> Velocity;
    typedef NamedQuantity<VelocityDimension> NamedVelocity;

    template<>
    class Quantity<VelocityDimension> : public QuantityBase<VelocityDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<VelocityDimension>& other) : BaseClass(other) {}
        Quantity(const NamedVelocity& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        static const NamedVelocity Knot;
        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    __declspec(selectany) const NamedVelocity Velocity::Knot(1852.0 / 3600.0, "Knot");
    const static NamedVelocity _Knot(1852.0 / 3600.0, "Knot");
    inline Velocity operator""_Knot(long double value) { return value * _Knot; }
    inline Velocity operator""_Knot(unsigned long long value) { return (double)value * _Knot; }

    template<typename CallbackT>
    static void Quantity<VelocityDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_Knot);
    }
}}}


