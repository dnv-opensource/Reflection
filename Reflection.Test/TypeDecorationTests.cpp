//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Types\DecoratedTypeDeducer.h"
#include "gtest\gtest.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types { namespace Tests {
    TEST(TypeDecorationTests, GetDecorationNameReturnsCorrectDecoration)
    {
        EXPECT_EQ("", GetDecorationName(TypeDecoration::None));
        EXPECT_EQ(" &", GetDecorationName(TypeDecoration::Reference));
        EXPECT_EQ(" const", GetDecorationName(TypeDecoration::Const));
        EXPECT_EQ(" const &", GetDecorationName(TypeDecoration::Const + TypeDecoration::Reference));
        EXPECT_EQ(" volatile", GetDecorationName(TypeDecoration::Volatile));
        EXPECT_EQ(" volatile &", GetDecorationName(TypeDecoration::Volatile + TypeDecoration::Reference));
        EXPECT_EQ(" volatile const", GetDecorationName(TypeDecoration::Volatile + TypeDecoration::Const));
        EXPECT_EQ(" volatile const &", GetDecorationName(TypeDecoration::Volatile + TypeDecoration::Const + TypeDecoration::Reference));
        EXPECT_EQ(" *", GetDecorationName(TypeDecoration::Pointer));
        EXPECT_EQ(" * &", GetDecorationName(TypeDecoration::Pointer + TypeDecoration::Reference));
        EXPECT_EQ(" * const", GetDecorationName(TypeDecoration::Pointer + TypeDecoration::Const));
        EXPECT_EQ(" ^", GetDecorationName(TypeDecoration::ManagedHandle));
        EXPECT_EQ(" %", GetDecorationName(TypeDecoration::ManagedReference));
        EXPECT_EQ(" ^%", GetDecorationName(TypeDecoration::ManagedHandle + TypeDecoration::ManagedReference));
    }
}}}}}