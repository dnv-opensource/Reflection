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
    typedef Dimension<-2, 0, 1, 0, 0> ForcePerUnitAreaDimension;
    class ForcePerUnitAreaPhenomenon : public IPhenomenon
    {
    public:
        typedef ForcePerUnitAreaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForcePerUnitAreaDimension());
        }
        static std::string GetName() { return "ForcePerUnitArea"; }
    };

    typedef Quantity<ForcePerUnitAreaPhenomenon> ForcePerUnitArea;
    typedef NamedQuantity<ForcePerUnitAreaDimension> NamedForcePerUnitArea;

    template<>
    class Quantity<ForcePerUnitAreaDimension> : public QuantityBase<ForcePerUnitAreaDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedForcePerUnitArea& other) : BaseClass(other) {}
        Quantity(const QuantityBase<ForcePerUnitAreaDimension>& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };    

    static const NamedForcePerUnitArea _Pa(1, "Pa");
    inline ForcePerUnitArea operator""_Pa(long double value) { return value * _Pa; }
    inline ForcePerUnitArea operator""_Pa(unsigned long long value) { return (double)value * _Pa; }

    static const NamedForcePerUnitArea _psi(6894.75729316759, "psi");
    inline ForcePerUnitArea operator""_psi(long double value) { return value * _psi; }
    inline ForcePerUnitArea operator""_psi(unsigned long long value) { return (double)value * _psi; }

    static const NamedForcePerUnitArea _kPa(1.e3, "kPa");
    inline ForcePerUnitArea operator""_kPa(long double value) { return value * _kPa; }
    inline ForcePerUnitArea operator""_kPa(unsigned long long value) { return (double)value * _kPa; }

    static const NamedForcePerUnitArea _MPa(1.e6, "MPa");
    inline ForcePerUnitArea operator""_MPa(long double value) { return value * _MPa; }
    inline ForcePerUnitArea operator""_MPa(unsigned long long value) { return (double)value * _MPa; }

    static const NamedForcePerUnitArea _GPa(1.e9, "GPa");
    inline ForcePerUnitArea operator""_GPa(long double value) { return value * _GPa; }
    inline ForcePerUnitArea operator""_GPa(unsigned long long value) { return (double)value * _GPa; }

    static const NamedForcePerUnitArea _ksi(6894.75729316759e3, "ksi");
    inline ForcePerUnitArea operator""_ksi(long double value) { return value * _ksi; }
    inline ForcePerUnitArea operator""_ksi(unsigned long long value) { return (double)value * _ksi; }

    static const NamedForcePerUnitArea _bar(1.e5, "bar");
    inline ForcePerUnitArea operator""_bar(long double value) { return value * _bar; }
    inline ForcePerUnitArea operator""_bar(unsigned long long value) { return (double)value * _bar; }

    static const NamedForcePerUnitArea _atm(101325.0, "atm");
    inline ForcePerUnitArea operator""_atm(long double value) { return value * _atm; }
    inline ForcePerUnitArea operator""_atm(unsigned long long value) { return (double)value * _atm; }

    template<typename CallbackT>
    static void Quantity<ForcePerUnitAreaDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_GPa);
        callback(_MPa);
        callback(_kPa);
        callback(_Pa);
        callback(_psi);
        callback(_ksi);
        callback(_bar);
        callback(_atm);
    }

}}}
