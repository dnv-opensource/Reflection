#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Length.h"
#include "Time.h"
#include "Force.h"
#include "Mass.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-1, 2, 1, 0, 0> CoupledQuadraticDampingDimension;
    class CoupledQuadraticDampingPhenomenon : public IPhenomenon
    {
    public:
        typedef CoupledQuadraticDampingDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * Pow(callback(TimeDimension()), 2) / callback(LengthDimension());
        }
        static std::string GetName() { return "CoupledQuadraticDamping"; }
    };

    typedef Quantity<CoupledQuadraticDampingPhenomenon> CoupledQuadraticDamping;
}}}


