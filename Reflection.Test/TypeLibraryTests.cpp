//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Types;
using namespace DNVS::MoFa::Reflection::TypeConversions;

TEST(TypeLibraryTests,TestFormattingOfRegisteredType)
{
    auto typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    typeLibrary->CreateType(typeid(int),"Integer");
    EXPECT_EQ("Integer *", typeLibrary->GetTypeFormatter()->FormatType(TypeId<int*>()));
}