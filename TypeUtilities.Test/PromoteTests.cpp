//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
#include "TypeUtilities\Promote.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    TEST(PromoteTests,TestPromote)
    {
        EXPECT_TRUE((std::is_same<Promote<short>::type,int>::value));
        EXPECT_TRUE((std::is_same<Promote<const short>::type,const int>::value));
        EXPECT_TRUE((std::is_same<Promote<volatile char>::type,volatile int>::value));
        EXPECT_TRUE((std::is_same<Promote<short&>::type,short&>::value));
        EXPECT_TRUE((std::is_same<Promote<const float>::type,const double>::value));
    }
}}}