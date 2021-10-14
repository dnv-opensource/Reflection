//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "TypeUtilities/ForEach.h"

namespace DNVS { namespace MoFa { namespace TypeUtilities { namespace Tests {
    TEST(ForEachTests,TestParallelFor_AllItemsTouched)
    {
        std::vector<double> data(1000, 0);
        ForEach(true, data.begin(), data.end(), [](double& data) {data = 1; });
        for (size_t i = 0; i < 1000; ++i)
            EXPECT_EQ(data[i], 1) << i;
    }

} } } }