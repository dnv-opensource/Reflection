#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"
#include "Length.h"
#include "Time.h"
#include "Force.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-2, 1, 1, 0, 0> DampingPerUnitLengthDimension;
    class DampingPerUnitLengthPhenomenon : public IPhenomenon
    {
    public:
        typedef DampingPerUnitLengthDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension())*callback(TimeDimension())/Pow(callback(LengthDimension()), 2);
        }
        static std::string GetName() { return "DampingPerUnitLength"; }
    };

    typedef Quantity<DampingPerUnitLengthPhenomenon> DampingPerUnitLength;
}}}

