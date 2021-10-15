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
    typedef Dimension<-1, 2, 1, 0, 0> MassDimension;
    class MassPhenomenon : public IPhenomenon
    {
    public:
        typedef MassDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(MassDimension());
        }
        static std::string GetName() { return "Mass"; }
    };

    typedef Quantity<MassPhenomenon> Mass;
    typedef NamedQuantity<MassDimension> NamedMass;

    template<>
    class Quantity<MassDimension> : public QuantityBase<MassDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<MassDimension>& other) : BaseClass(other) {}
        Quantity(const NamedMass& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    const static NamedMass _Gg(1e6, "Gg");
    inline Mass operator""_Gg(long double value) { return value * _Gg; }
    inline Mass operator""_Gg(unsigned long long value) { return (double)value * _Gg; }

    const static NamedMass _Mg(1e6, "Mg");
    inline Mass operator""_Mg(long double value) { return value * _Mg; }
    inline Mass operator""_Mg(unsigned long long value) { return (double)value * _Mg; }

    const static NamedMass _kilotonne(1e6, "kilotonne");
    inline Mass operator""_kilotonne(long double value) { return value * _kilotonne; }
    inline Mass operator""_kilotonne(unsigned long long value) { return (double)value * _kilotonne; }

    const static NamedMass _kg(1, "kg");
    inline Mass operator""_kg(long double value) { return value * _kg; }
    inline Mass operator""_kg(unsigned long long value) { return (double)value * _kg; }

    const static NamedMass _g(1e-3, "g");
    inline Mass operator""_g(long double value) { return value * _g; }
    inline Mass operator""_g(unsigned long long value) { return (double)value * _g; }

    const static NamedMass _hg(1e-1, "hg");
    inline Mass operator""_hg(long double value) { return value * _hg; }
    inline Mass operator""_hg(unsigned long long value) { return (double)value * _hg; }

    const static NamedMass _lb(0.45359237, "lb");
    inline Mass operator""_lb(long double value) { return value * _lb; }
    inline Mass operator""_lb(unsigned long long value) { return (double)value * _lb; }

    const static NamedMass _tonne(1e3, "tonne");
    inline Mass operator""_tonne(long double value) { return value * _tonne; }
    inline Mass operator""_tonne(unsigned long long value) { return (double)value * _tonne; }

    const static NamedMass _ton(1016.0469088, "ton");
    inline Mass operator""_ton(long double value) { return value * _ton; }
    inline Mass operator""_ton(unsigned long long value) { return (double)value * _ton; }

    const static NamedMass _kips(453.59237, "kips");
    inline Mass operator""_kips(long double value) { return value * _kips; }
    inline Mass operator""_kips(unsigned long long value) { return (double)value * _kips; }

    const static NamedMass _slug(14.5939035919985, "slug");
    inline Mass operator""_slug(long double value) { return value * _slug; }
    inline Mass operator""_slug(unsigned long long value) { return (double)value * _slug; }

    template<typename CallbackT>
    static void Quantity<MassDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_g);
        callback(_hg);
        callback(_kg);
        callback(_lb);
        callback(_tonne);
        callback(_kilotonne);
        callback(_ton);
        callback(_kips);
        callback(_slug);
        callback(_Mg);
        callback(_Gg);
    }
}}}
