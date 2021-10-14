#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "TempDiff.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 0, 0, -1, 0> ThermalExpansionCoeffDimension;
    class ThermalExpansionCoeffPhenomenon : public IPhenomenon
    {
    public:
        typedef ThermalExpansionCoeffDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(TempDiffDimension()),-1);
        }
        static std::string GetName() { return "ThermalExpansionCoeff"; }
    };

    typedef Quantity<ThermalExpansionCoeffPhenomenon> ThermalExpansionCoeff;
}}}

