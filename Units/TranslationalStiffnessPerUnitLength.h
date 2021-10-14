#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Quantity.h"
#include "ForcePerUnitArea.h"

namespace DNVS {namespace MoFa {namespace Units {
    typedef Dimension<-2,0,1,0,0> TranslationalStiffnessPerUnitLengthDimension;
    class TranslationalStiffnessPerUnitLengthPhenomenon : public IPhenomenon
    {
    public:
        typedef TranslationalStiffnessPerUnitLengthDimension DimensionType;        
        template<typename ReturnTypeT, typename CallbackT>
        static ReturnTypeT ComposeUnit(CallbackT& callback)
        {
            return callback(ForcePerUnitAreaDimension());
        }
        static std::string GetName() { return "TranslationalStiffnessPerUnitLength"; }
    };

    typedef Quantity<TranslationalStiffnessPerUnitLengthPhenomenon> TranslationalStiffnessPerUnitLength;
}}}

