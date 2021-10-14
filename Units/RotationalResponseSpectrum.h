#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "Angle.h"
#include "Time.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<0, 1, 0, 0, 2> RotationalResponseSpectrumDimension;
    class RotationalResponseSpectrumPhenomenon : public IPhenomenon
    {
    public:
        typedef RotationalResponseSpectrumDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return Pow(callback(AngleDimension()),2) * callback(TimeDimension());
        }
        static std::string GetName() { return "RotationalResponseSpectrum"; }
    };

    typedef Quantity<RotationalResponseSpectrumPhenomenon> RotationalResponseSpectrum;
}}}

