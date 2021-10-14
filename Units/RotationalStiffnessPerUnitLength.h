#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
#include "IPhenomenon.h"
#include "Force.h"

namespace DNVS {namespace MoFa {namespace Units {

    typedef Dimension<0, 0, 1, 0, 0> RotationalStiffnessPerUnitLengthDimension;
    class RotationalStiffnessPerUnitLengthPhenomenon : public IPhenomenon
    {
    public:
        typedef RotationalStiffnessPerUnitLengthDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension());
        }
        static std::string GetName() { return "RotationalStiffnessPerUnitLength"; }
    };

    typedef Quantity<RotationalStiffnessPerUnitLengthPhenomenon> RotationalStiffnessPerUnitLength;

}}}