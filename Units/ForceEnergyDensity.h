#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Force.h"
#include "Time.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 1, 2, 0, 0> ForceEnergyDensityDimension;
    class ForceEnergyDensityPhenomenon : public IPhenomenon
    {
    public:
        typedef ForceEnergyDensityDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(ForceDimension()), 2) * callback(TimeDimension());
        }
        static std::string GetName() { return "ForceEnergyDensity"; }
    };

    typedef Quantity<ForceEnergyDensityPhenomenon> ForceEnergyDensity;
}}}

