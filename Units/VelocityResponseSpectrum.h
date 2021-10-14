#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "IPhenomenon.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<2,-1,0,0,0> VelocityResponseSpectrumDimension;
    class VelocityResponseSpectrumPhenomenon : public IPhenomenon
    {
    public:
        typedef VelocityResponseSpectrumDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(LengthDimension()),2) / callback(TimeDimension());
        }
        static std::string GetName() { return "VelocityResponseSpectrum"; }
    };

    typedef Quantity<VelocityResponseSpectrumPhenomenon> VelocityResponseSpectrum;
}}}

