#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "ForcePerUnitArea.h"
#include "Angle.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-2, 0, 1, 0, -1> ForcePerUnitAreaPerAngleDimension;
    class ForcePerUnitAreaPerAnglePhenomenon : public IPhenomenon
    {
    public:
        typedef ForcePerUnitAreaPerAngleDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForcePerUnitAreaDimension())/callback(AngleDimension());
        }
        static std::string GetName() { return "ForcePerUnitAreaPerAngle"; }
    };
    typedef Quantity<ForcePerUnitAreaPerAnglePhenomenon> ForcePerUnitAreaPerAngle;
}}}


