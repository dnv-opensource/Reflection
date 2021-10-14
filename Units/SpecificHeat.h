#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Force.h"
#include "Length.h"
#include "Mass.h"
#include "TempDiff.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<2, -2, 0, -1, 0> SpecificHeatDimension;
    class SpecificHeatPhenomenon : public IPhenomenon
    {
    public:
        typedef SpecificHeatDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * callback(LengthDimension()) / callback(MassDimension()) / callback(TempDiffDimension());
        }
        static std::string GetName() { return "SpecificHeat"; }
    };

    typedef Quantity<SpecificHeatPhenomenon> SpecificHeat;
}}}

