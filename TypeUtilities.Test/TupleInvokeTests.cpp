//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "TypeUtilities/TupleInvoke.h"
#include <type_traits>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    TEST(TupleInvokeTests,InvokeFunctionWithNoArguments)
    {        
        auto fn = []()->double {return 0.0;};
        EXPECT_TRUE((std::is_same<decltype(TupleInvoke(fn, std::make_tuple())),double>::value));
    }
    TEST(TupleInvokeTests,InvokeFunctionWithOneArgument)
    {        
        auto fn = [](double)->double {return 0.0;};
        EXPECT_TRUE((std::is_same<decltype(TupleInvoke(fn, std::make_tuple(9.8))),double>::value));
    }
    TEST(TupleInvokeTests,InvokeFunctionWithOneArgument_Reference)
    {        
        auto fn = [](double& arg)->double {return arg;};
        double arg=4.5;
        EXPECT_DOUBLE_EQ(arg,TupleInvoke(fn,std::tuple<double&>(arg)));
    }

    TEST(TupleInvokeTests,InvokeFunctionWithTenArguments)
    {        
        auto fn = [](int a, int b, int c, int d, int e, int f, int g, int h, int i, int j)->int {return a+b+c+d+e+f+g+h+i+j;};        
        EXPECT_EQ(11*5,TupleInvoke(fn,std::make_tuple(1,2,3,4,5,6,7,8,9,10)));
    }

    TEST(TupleInvokeTests, InvokeGetReturnTypeElevenArguments)
    {
        auto fn = [](int a, int b, int c, int d, int e, int f, int g, int h, int i, int j,int k)->int {return a + b + c + d + e + f + g + h + i + j + k; };
        auto tuple = std::make_tuple(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
        static_assert(std::is_same_v<int, typename TupleInvokeUtility<decltype(fn), decltype(tuple)>::ReturnType>, "Incorrect return type detection");
    }

}}}