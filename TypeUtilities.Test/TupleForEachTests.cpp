//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "TypeUtilities/TupleForEach.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    TEST(TupleForEachTests,ForEachEmptyTuple)
    {
        int count=0;
        auto fn = [&count](int) {++count;};
        TupleForEach(std::make_tuple(), fn);
        EXPECT_EQ(0, count);
    }
    TEST(TupleForEachTests,ForEachWithThreeArguments)
    {        
        int count=0;
        auto fn = [&count](int) {++count;};
        TupleForEach(std::make_tuple(1, 2, 3), fn);
        EXPECT_EQ(3, count);
    }

}}}