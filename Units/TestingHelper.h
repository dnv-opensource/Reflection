#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Quantity.h"
namespace testing { namespace internal {

    template<typename DimensionT>
    AssertionResult DoubleNearPredFormat(const char* expr1,
        const char* expr2,
        const char* abs_error_expr,
        const DNVS::MoFa::Units::Quantity<DimensionT>& val1,
        const DNVS::MoFa::Units::Quantity<DimensionT>& val2,
        double abs_error)
    {
        return DoubleNearPredFormat(expr1, expr2, abs_error_expr, val1.GetValue(), val2.GetValue(), abs_error);
    }

    #define EXPECT_QUANTITY_EQ(val1,val2) EXPECT_DOUBLE_EQ((val1).GetValue(),(val2).GetValue())
    #define EXPECT_QUANTITY_NEAR(val1,val2, diff) EXPECT_NEAR((val1).GetValue(),(val2).GetValue(), diff)

}}