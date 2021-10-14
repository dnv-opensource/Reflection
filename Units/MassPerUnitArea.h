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
    typedef Dimension<-3, 2, 1, 0, 0> MassPerUnitAreaDimension;
    class MassPerUnitAreaPhenomenon : public IPhenomenon
    {
    public:
        typedef MassPerUnitAreaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(MassDimension()) / Pow(callback(LengthDimension()),2);
        }
        static std::string GetName() { return "MassPerUnitArea"; }
    };

    typedef Quantity<MassPerUnitAreaPhenomenon> MassPerUnitArea;
}}}

