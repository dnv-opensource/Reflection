#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Length.h"
#include "Force.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-1, 0, -1, 0, 0> RotationalFlexibilityDimension;
    class RotationalFlexibilityPhenomenon : public IPhenomenon
    {
    public:
        typedef RotationalFlexibilityDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()),-1) / callback(ForceDimension());
        }
        static std::string GetName() { return "RotationalFlexibility"; }
    };

    typedef Quantity<RotationalFlexibilityPhenomenon> RotationalFlexibility;
}}}


