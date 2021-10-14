//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "TypeUtilities\ThreadPool.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities { namespace Tests {
    TEST(ThreadPoolTests, ParallelFor)
    {
        ThreadPool tp;
        size_t count = 10000;
        std::vector<int> v(count, 3);
        std::atomic<int> sum(0);
        tp.ForEach(v.begin(), v.end(), [&](int val) {
            sum += val; 
        });
        EXPECT_DOUBLE_EQ(sum, count *3);
    }
    TEST(ThreadPoolTests, ParallelForNested)
    {
        ThreadPool tp;
        size_t count = 100;
        std::vector<int> v(count, 3);
        std::atomic<int> sum(0);
        tp.ForEach(v.begin(), v.end(), [&](int val) {
            tp.ForEach(v.begin(), v.end(), [&](int val2) { sum += val * val2; });
        });
        EXPECT_DOUBLE_EQ(sum, count * count * 3 * 3);
    }

}}}}