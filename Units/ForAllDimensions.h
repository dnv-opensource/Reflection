#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Dimension.h"
#include "LambdaApplier.h"

namespace DNVS {namespace MoFa {namespace Units {

    template<typename Operation>
    void ForAllUsedDimensions(Operation& op) 
    {
        op.Apply<Dimension<-4, 2, 1, 0, 0>>();
        op.Apply<Dimension<-3, 2, 1, 0, 0>>();
        op.Apply<Dimension<-3, 0, 1, 0, 0>>();
        op.Apply<Dimension<-2, 1, 1, 0, 0>>();
        op.Apply<Dimension<-2, 1, 0, 0, 0>>();
        op.Apply<Dimension<-2, 2, 1, 0, 0>>();
        op.Apply<Dimension<-2, 0, 1, 0, -1>>();
        op.Apply<Dimension<-2, 0, 1, 0, 0>>();
        op.Apply<Dimension<-1, 1, 1, 0, 0>>();
        op.Apply<Dimension<-1, 2, 1, 0, 0>>();
        op.Apply<Dimension<-1, 0, -1, 0, 0>>();
        op.Apply<Dimension<-1, 0, 1, 0, 0>>();
        op.Apply<Dimension<-1, 0, 0, 1, 0>>();
        op.Apply<Dimension<-1, 0, 0, 0, 1>>();
        op.Apply<Dimension<-1, 0, 0, 0, 0>>();
        op.Apply<Dimension<1, -2, 0, 0, 0>>();
        op.Apply<Dimension<1, -1, 0, 0, 0>>();
        op.Apply<Dimension<1, 1, 1, 0, 0>>();
        op.Apply<Dimension<1, 2, 1, 0, 0>>();
        op.Apply<Dimension<1, 0, -1, 0, 0>>();
        op.Apply<Dimension<1, 0, 1, 0, 0>>();
        op.Apply<Dimension<1, 0, 0, 0, 0>>();
        op.Apply<Dimension<2, -2, 0, -1, 0>>();
        op.Apply<Dimension<2, -1, 0, 0, 0>>();
        op.Apply<Dimension<2, 1, 2, 0, 0>>();
        op.Apply<Dimension<2, 1, 0, 0, 0>>();
        op.Apply<Dimension<2, 0, 1, 0, 1>>();
        op.Apply<Dimension<2, 0, 1, 0, -1>>();
        op.Apply<Dimension<2, 0, 1, 0, 0>>();
        op.Apply<Dimension<2, 0, 0, 0, 0>>();
        op.Apply<Dimension<3, 0, 0, 0, 0>>();
        op.Apply<Dimension<4, 0, 0, 0, 0>>();
        op.Apply<Dimension<0, -2, 0, 0, 1>>();
        op.Apply<Dimension<0, -1, 0, 0, 1>>();
        op.Apply<Dimension<0, -1, 0, 0, 0>>();
        op.Apply<Dimension<0, 1, 1, 0, 0>>();
        op.Apply<Dimension<0, 1, 2, 0, 0>>();
        op.Apply<Dimension<0, 1, 0, 0, 2>>();
        op.Apply<Dimension<0, 1, 0, 0, 0>>();
        op.Apply<Dimension<0, 2, 1, 0, 0>>();
        op.Apply<Dimension<0, 0, 1, 0, 0>>();
        op.Apply<Dimension<0, 0, 0, -1, 0>>();
        op.Apply<Dimension<0, 0, 0, 1, 0>>();
        op.Apply<Dimension<0, 0, 0, 0, 1>>();
    }

    template<typename LambdaT>
    void ForAllUsedDimensionsLambda(const LambdaT& op)
    {
        LambdaApplier<LambdaT> applier(op);
        ForAllUsedDimensions(applier);
    }

}}}
