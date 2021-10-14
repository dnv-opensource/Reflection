//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Variants/InternalVariantService.h"
#include <memory>

namespace DNVS { namespace MoFa { namespace Reflection {namespace Variants {namespace Test {
    TEST(InternalVariantServiceTests, SetData_UnreflectUniquePtr)
    {
        std::unique_ptr<int> ptr(new int);
        *ptr = 55;
        Variant v;
        InternalVariantService::SetVariantData<std::unique_ptr<int>>(v, std::move(ptr));
        int* u = InternalVariantService::UnreflectUnchecked<int*>(v);
        EXPECT_EQ(55, *u);
        std::unique_ptr<int>& var = InternalVariantService::UnreflectUnchecked<std::unique_ptr<int>>(v);
        EXPECT_EQ(55, *var);
    }
}}}}}