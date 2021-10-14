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
    typedef Dimension<0, 2, 1, 0, 0> CoupledQuadraticDampingForceDimension;
    class CoupledQuadraticDampingForcePhenomenon : public IPhenomenon
    {
    public:
        typedef CoupledQuadraticDampingForceDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * Pow(callback(TimeDimension()), 2);
        }
        static std::string GetName() { return "CoupledQuadraticDampingForce"; }
    };

    typedef Quantity<CoupledQuadraticDampingForcePhenomenon> CoupledQuadraticDampingForce;
}}}

