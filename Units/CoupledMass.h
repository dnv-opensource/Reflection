#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"
#include "Mass.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 2, 1, 0, 0> CoupledMassDimension;
    class CoupledMassPhenomenon : public IPhenomenon
    {
    public:
        typedef CoupledMassDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(MassDimension())*callback(LengthDimension());
        }
        static std::string GetName() { return "CoupledMass"; }
    };

    typedef Quantity<CoupledMassPhenomenon> CoupledMass;

}}}

