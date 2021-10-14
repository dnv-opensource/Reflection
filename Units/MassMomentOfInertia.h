#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Mass.h"
#include "Length.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<1, 2, 1, 0, 0> MassMomentOfInertiaDimension;
    class MassMomentOfInertiaPhenomenon : public IPhenomenon
    {
    public:
        typedef MassMomentOfInertiaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(MassDimension()) * Pow(callback(LengthDimension()), 2);
        }
        static std::string GetName() { return "MassMomentOfInertia"; }
    };

    typedef Quantity<MassMomentOfInertiaPhenomenon> MassMomentOfInertia;
}}}

