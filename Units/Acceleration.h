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
    typedef Dimension<1, -2, 0, 0, 0> AccelerationDimension;
    class AccelerationPhenomenon : public IPhenomenon
    {
    public:
        typedef AccelerationDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(LengthDimension())/Pow(callback(TimeDimension()),2);
        }
        static std::string GetName() {return "Acceleration";}
    };

    typedef Quantity<AccelerationPhenomenon> Acceleration;
    typedef NamedQuantity<AccelerationDimension> NamedAcceleration;

    template<>
    class Quantity<AccelerationDimension> : public QuantityBase<AccelerationDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedAcceleration& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        ///G is Earths acceleration.
        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    const static NamedAcceleration _G(9.80665, "G");
    inline Acceleration operator""_G(long double value) { return value * _G; }
    inline Acceleration operator""_G(unsigned long long value) { return (double)value * _G; }
    
    template<typename CallbackT>
    static void Quantity<AccelerationDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_G);
    }
}}}

