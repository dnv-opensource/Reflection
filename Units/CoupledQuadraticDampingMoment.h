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
#include "Mass.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-1, 2, 1, 0, 0> CoupledQuadraticDampingMomentDimension;
    class CoupledQuadraticDampingMomentPhenomenon : public IPhenomenon
    {
    public:
        typedef CoupledQuadraticDampingMomentDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * Pow(callback(TimeDimension()), 2) / callback(LengthDimension());
        }
        static std::string GetName() { return "CoupledQuadraticDampingMoment"; }
    };

    typedef Quantity<CoupledQuadraticDampingMomentPhenomenon> CoupledQuadraticDampingMoment;
}}}


