#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"
#include "Force.h"
#include "Length.h"
#include "Energy.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<1, 0, 1, 0, 0> MomentOfForceDimension;   
    
    class MomentOfForcePhenomenon : public IPhenomenon
    {
    public:
        typedef MomentOfForceDimension DimensionType;
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForceDimension()) * callback(LengthDimension());
        }
        static std::string GetName() {return "MomentOfForce";}
    };
    typedef Quantity<MomentOfForcePhenomenon> MomentOfForce;

}}}


