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
    typedef Dimension<1, 0, 1, 0, 0> EnergyDimension;   
    class EnergyPhenomenon : public IPhenomenon
    {
    public:
        typedef EnergyDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(EnergyDimension());
        }
        static std::string GetName() {return "Energy";}
    };

    typedef Quantity<EnergyPhenomenon> Energy;
    typedef NamedQuantity<EnergyDimension> NamedEnergy;

    template<>
    class Quantity<EnergyDimension > : public QuantityBase<EnergyDimension > 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedEnergy& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity() {}  
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };

    const static NamedEnergy _J(1, "J");
    inline Energy operator""_J(long double value) { return value * _J; }
    inline Energy operator""_J(unsigned long long value) { return (double)value * _J; }

    const static NamedEnergy _kJ(1e3, "kJ");
    inline Energy operator""_kJ(long double value) { return value * _kJ; }
    inline Energy operator""_kJ(unsigned long long value) { return (double)value * _kJ; }

    const static NamedEnergy _MJ(1e6, "MJ");
    inline Energy operator""_MJ(long double value) { return value * _MJ; }
    inline Energy operator""_MJ(unsigned long long value) { return (double)value * _MJ; }

    const static NamedEnergy _GJ(1e9, "GJ");
    inline Energy operator""_GJ(long double value) { return value * _GJ; }
    inline Energy operator""_GJ(unsigned long long value) { return (double)value * _GJ; }

    const static NamedEnergy _Btu(1055.05585262, "Btu");
    inline Energy operator""_Btu(long double value) { return value * _Btu; }
    inline Energy operator""_Btu(unsigned long long value) { return (double)value * _Btu; }

    template<typename CallbackT>
    static void Quantity<EnergyDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_GJ);
        callback(_MJ);
        callback(_kJ);
        callback(_J);
        callback(_Btu);
    }

}}}


