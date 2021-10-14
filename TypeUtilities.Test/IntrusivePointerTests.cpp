//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "TypeUtilities/IntrusiveClass.h"
#include <ppl.h>
#include <iostream>
#include "TypeUtilities/SmartPointerHelper.h"

namespace DNVS { namespace MoFa {namespace TypeUtilities { namespace Tests {
    class TestObject : public IntrusiveClass<TestObject>
    {
    public:
        TestObject(std::function<void()> f)
            : m_f(f)
        {}
        virtual ~TestObject()
        {
            m_f();
        }
    private:
        std::function<void()> m_f;
    };
    
    TEST(IntrusivePointerTests, TestDeallocatorFromTwoThreads)
    {
        bool canDelete = false;
        auto callback = [&canDelete]() { EXPECT_TRUE(canDelete); };

        IntrusivePointer<TestObject> a = new TestObject(callback);
        auto worker1 = [&a]() 
        {
            for (size_t i = 0; i < 100; ++i)
            {
                IntrusivePointer<TestObject> b = a;
            }
        };
        auto worker2 = [&a]()
        {
            for (size_t i = 0; i < 100; ++i)
            {
                IntrusivePointer<TestObject> b = a;
            }
        };
        Concurrency::parallel_invoke(worker1, worker2);
        canDelete = true;
    }

    TEST(IntrusivePointerTests, SmartPointerOfboostOptional)
    {
        EXPECT_TRUE(TypeUtilities::is_smart_pointer<std::unique_ptr<boost::optional<int>>>::value);
    }
}}}}