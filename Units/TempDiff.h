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
    typedef Dimension<0, 0, 0, 1, 0> TempDiffDimension;
    class TempDiffPhenomenon : public IPhenomenon
    {
    public:
        typedef TempDiffDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(TempDiffDimension());
        }
        static std::string GetName() { return "TempDiff"; }
    };

    typedef Quantity<TempDiffPhenomenon> TempDiff;
    typedef NamedQuantity<TempDiffDimension> NamedTempDiff;

    template<>
    class Quantity<TempDiffDimension> : public QuantityBase<TempDiffDimension> 
    {
    public:
        Quantity(const Quantity& other) : BaseClass(other) {}
        Quantity(const QuantityBase<TempDiffDimension>& other) : BaseClass(other) {}
        Quantity(const NamedTempDiff& other) : BaseClass(other) {}
        explicit Quantity(double value) : BaseClass(value) {}
        Quantity(){}
        Quantity& operator=(const Quantity& other) {BaseClass::operator=(other); return *this; }

        template<typename CallbackT>
        static void ForEachNamedUnit(CallbackT& callback);
    };
    const static NamedTempDiff _delC(1, "delC");
    inline TempDiff operator""_delC(long double value) { return value * _delC; }
    inline TempDiff operator""_delC(unsigned long long value) { return (double)value * _delC; }

    const static NamedTempDiff _delK(1, "delK");
    inline TempDiff operator""_delK(long double value) { return value * _delK; }
    inline TempDiff operator""_delK(unsigned long long value) { return (double)value * _delK; }

    const static NamedTempDiff _delF(5. / 9., "delF");
    inline TempDiff operator""_delF(long double value) { return value * _delF; }
    inline TempDiff operator""_delF(unsigned long long value) { return (double)value * _delF; }

    template<typename CallbackT>
    static void Quantity<TempDiffDimension>::ForEachNamedUnit(CallbackT& callback)
    {
        callback(_delC);
        callback(_delK);
        callback(_delF);
    }
}}}
