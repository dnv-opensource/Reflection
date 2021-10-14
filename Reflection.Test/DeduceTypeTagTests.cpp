//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/Types/DeduceTypeTag.h"
//#include "Utility/SmartPointer.h"

namespace DNVS { namespace MoFa { namespace Reflection {namespace Types {namespace Test {
    struct SmallPodType {
        int member;
        char member2;
    };
    struct LargePodType {
        int member;
        double member2;
    };
    enum DummyEnum {
        Red = 5
    };
    TEST(TypeTraitsTests, TestSmallPodTypes)
    {
        static_assert(std::is_same_v<TypeTag<int>, SmallPodTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<DummyEnum>, SmallPodTag>, "Invalid type tag");
#ifdef _WIN64
        static_assert(std::is_same_v<TypeTag<double>, SmallPodTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<SmallPodType>, SmallPodTag>, "Invalid type tag");
#endif
        static_assert(!std::is_same_v<TypeTag<LargePodType>, SmallPodTag>, "Invalid type tag");
        static_assert(!std::is_same_v<TypeTag<void>, SmallPodTag>, "Invalid type tag");
        static_assert(!std::is_same_v<TypeTag<void*>, SmallPodTag>, "Invalid type tag");
        static_assert(!std::is_same_v<TypeTag<int&>, SmallPodTag>, "Invalid type tag");
    }

    TEST(TypeTraitsTests, TestSmartPointerTypes)
    {
        static_assert(std::is_same_v<TypeTag<std::shared_ptr<int>>, AliasingSmartPointerTag>, "Invalid type tag");
//        static_assert(std::is_same_v<TypeTag<Utility::SmartPointer<int>>, AliasingSmartPointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<std::unique_ptr<int>>, MovableSmartPointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<const std::unique_ptr<int>&>, MovableSmartPointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<std::unique_ptr<int>&&>, MovableSmartPointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<std::shared_ptr<int>&>, AliasingSmartPointerTag>, "Invalid type tag");
        static_assert(!std::is_same_v<TypeTag<std::shared_ptr<int>*>, AliasingSmartPointerTag>, "Invalid type tag");
    }

    TEST(TypeTraitsTests, TestPointerTypes)
    {
        static_assert(std::is_same_v<TypeTag<int*>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<double*>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<std::shared_ptr<int>*>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<const int*>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<const int*&>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<const int[4]>, PointerTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<decltype("Hei")>, PointerTag>, "Invalid type tag");
    }

    TEST(TypeTraitsTests, TestReferenceTypes)
    {
        static_assert(std::is_same_v<TypeTag<int&>, ReferenceTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<const double&>, ReferenceTag>, "Invalid type tag");
    }

    TEST(TypeTraitsTests, TestValueTypes)
    {
        static_assert(std::is_same_v<TypeTag<std::string>, ValueTag>, "Invalid type tag");
        static_assert(std::is_same_v<TypeTag<LargePodType>, ValueTag>, "Invalid type tag");
    }
}}}}}