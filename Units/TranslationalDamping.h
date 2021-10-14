#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Force.h"
#include "Time.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-1, 1, 1, 0, 0> TranslationalDampingDimension;
    class TranslationalDampingPhenomenon : public IPhenomenon
    {
    public:
        typedef TranslationalDampingDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * callback(TimeDimension()) / callback(LengthDimension());
        }
        static std::string GetName() { return "TranslationalDamping"; }
    };

    typedef Quantity<TranslationalDampingPhenomenon> TranslationalDamping;
}}}

