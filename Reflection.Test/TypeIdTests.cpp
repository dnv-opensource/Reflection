//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Types\DecoratedTypeInfo.h"

#include "boost\lexical_cast.hpp"
#include <memory>
#include "Reflection\Types\DefaultTypeFormatter.h"
#include "Reflection\Types\DecoratedTypeFormatter.h"
#include "gtest\gtest.h"
//#include "Utility\SmartPointer.h"

using namespace DNVS::MoFa::Reflection::Types;

TEST(TypeIdTests, TypeId_Int)
{
    DecoratedTypeInfo info = TypeId<int>();
    EXPECT_EQ(std::type_index(typeid(int)), info.GetTypeInfo());
    EXPECT_EQ(0, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_const_Int)
{
    DecoratedTypeInfo info = TypeId<const int>();
    EXPECT_EQ(std::type_index(typeid(int)), info.GetTypeInfo());
    EXPECT_EQ(2, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_int_pointer)
{
    DecoratedTypeInfo info = TypeId<int*>();
    EXPECT_EQ(std::type_index(typeid(int)), info.GetTypeInfo());
    EXPECT_EQ(8, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_int_reference)
{
    DecoratedTypeInfo info = TypeId<int&>();
    EXPECT_EQ(std::type_index(typeid(int)), info.GetTypeInfo());
    EXPECT_EQ(1, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_string)
{
    DecoratedTypeInfo info = TypeId<std::string>();
    EXPECT_EQ(std::type_index(typeid(std::string)), info.GetTypeInfo());
    //Treat all values as const ref.
    EXPECT_EQ(3, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_intptr_asArg)
{
    DecoratedTypeInfo info = TypeId<int*>();
    EXPECT_EQ(std::type_index(typeid(int)), info.GetTypeInfo());
    EXPECT_EQ(8, info.GetDecoration());
}

TEST(TypeIdTests, TypeId_stringRef_asArg)
{
    DecoratedTypeInfo info = TypeId<std::string&>();
    EXPECT_EQ(std::type_index(typeid(std::string)), info.GetTypeInfo());
    EXPECT_EQ(1, info.GetDecoration());
}

TEST(TypeIdTests, IndexedTypeInfo_Assign)
{
    DecoratedTypeInfo t = TypeId<double>();
    DecoratedTypeInfo t2(typeid(double), 0);
    //Mismatch between 32 bit and 64 bit
    EXPECT_EQ(t2.GetTypeInfo(), t.GetTypeInfo());
}

TEST(TypeIdTests, IndexedTypeInfo_CopyConstruct)
{
    DecoratedTypeInfo t(TypeId<double>());
}

TEST(TypeIdTests, StreamingOfName)
{
    DecoratedTypeInfo info = TypeId<unsigned char>();
    EXPECT_EQ("unsigned char", boost::lexical_cast<std::string>(info));
}

TEST(TypeIdTests, DeduceSmartPointer)
{
    EXPECT_FALSE(IsPointerWrapper<int>::value);
    EXPECT_FALSE(IsPointerWrapper<int*>::value);
    EXPECT_TRUE(IsPointerWrapper<std::shared_ptr<int>>::value);
    EXPECT_TRUE(IsPointerWrapper<std::unique_ptr<int>>::value);
    EXPECT_FALSE(IsPointerWrapper<std::unique_ptr<int>&>::value);
}

TEST(TypeIdTests, DeduceSmartPointerPointerType)
{
    typedef UnwrapPointer<std::shared_ptr<int>>::type pointer_type;
    EXPECT_TRUE((std::is_same<int*, pointer_type>::value));
}

TEST(TypeIdTests, DeduceSmartPointerAliasing)
{
    EXPECT_TRUE(IsAliasingSmartPointer<std::shared_ptr<int> >::value);
    EXPECT_FALSE(IsAliasingSmartPointer<std::unique_ptr<int> >::value);
}

TEST(TypeIdTests, DecoratedTypeFormatterTest)
{
    DecoratedTypeFormatter formatter(std::make_shared<DefaultTypeFormatter>());
    EXPECT_EQ("double &", formatter.FormatType(TypeId<double&>()));
}

TEST(TypeIdTests, TypeIdOfconstPointer_VerifyDecoration)
{
    EXPECT_EQ(40, TypeId<const double*>().GetDecoration());
}

TEST(TypeIdTests, TypeTagOfSmartPointers)
{
    //static_assert(std::is_same_v<AliasingSmartPointerTag, TypeTag<DNVS::MoFa::Utility::SmartPointer<int>>>, "Expected aliasing smart pointer tag.");
    static_assert(std::is_same_v<AliasingSmartPointerTag, TypeTag<std::shared_ptr<int>>>, "Expected aliasing smart pointer tag.");
    static_assert(std::is_same_v<MovableSmartPointerTag, TypeTag<std::unique_ptr<int>>>, "Expected move pointer tag.");
}
