#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"
#include "Volume.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<3, 0, 0, 0, 0> FirstMomentOfAreaDimension;
    class FirstMomentOfAreaPhenomenon : public IPhenomenon
    {
    public:
        typedef FirstMomentOfAreaDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()),3);
        }
        static std::string GetName() { return "FirstMomentOfArea"; }
    };

    typedef Quantity<FirstMomentOfAreaPhenomenon> FirstMomentOfArea;
}}}

