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
    typedef Dimension<0, 0, 1, 0, 0> ForceDimension;
    class ForcePhenomenon : public IPhenomenon
    {
    public:
        typedef ForceDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension());
        }
        static std::string GetName() { return "Force"; }
    };

    typedef Quantity<ForcePhenomenon> Force;
    typedef NamedQuantity<ForceDimension> NamedForce;

    template<>
    class Quantity<ForceDimension> : public QuantityBase<ForceDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const NamedForce& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    
    const static NamedForce _N(1, "N");
    inline Force operator""_N(long double value) { return value * _N; }
    inline Force operator""_N(unsigned long long value) { return (double)value * _N; }

    const static NamedForce _kN(1e3, "kN");
    inline Force operator""_kN(long double value) { return value * _kN; }
    inline Force operator""_kN(unsigned long long value) { return (double)value * _kN; }

    const static NamedForce _MN(1e6, "MN");
    inline Force operator""_MN(long double value) { return value * _MN; }
    inline Force operator""_MN(unsigned long long value) { return (double)value * _MN; }

    const static NamedForce _GN(1e9, "GN");
    inline Force operator""_GN(long double value) { return value * _GN; }
    inline Force operator""_GN(unsigned long long value) { return (double)value * _GN; }

    const static NamedForce _mN(1e-3, "mN");
    inline Force operator""_mN(long double value) { return value * _mN; }
    inline Force operator""_mN(unsigned long long value) { return (double)value * _mN; }

    const static NamedForce _lbf(4.44822161526, "lbf");
    inline Force operator""_lbf(long double value) { return value * _lbf; }
    inline Force operator""_lbf(unsigned long long value) { return (double)value * _lbf; }

    const static NamedForce _kipf(4.44822161526e3, "kipf");
    inline Force operator""_kipf(long double value) { return value * _kipf; }
    inline Force operator""_kipf(unsigned long long value) { return (double)value * _kipf; }

    const static NamedForce _kgf(9.80665, "kgf");
    inline Force operator""_kgf(long double value) { return value * _kgf; }
    inline Force operator""_kgf(unsigned long long value) { return (double)value * _kgf; }

    template<typename CallbackT>
    static void Quantity<ForceDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_GN);
        callback(_MN);
        callback(_kN);
        callback(_N);
        callback(_lbf);
        callback(_kipf);
        callback(_kgf);
    }

}}}
