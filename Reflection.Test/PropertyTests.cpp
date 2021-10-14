//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"
#include "Reflection\Properties\DefineProperties.h"
#include "Reflection\Attributes\AttributeCollection.h"
#include "gtest\gtest.h"

using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Properties;



TEST(PropertyTests,CreateDefineProperties)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineProperties<std::string> cls(typeLibrary,typeLibrary->CreateType(typeid(std::string), "string"));
    cls.Get("length",&std::string::length);
}

TEST(PropertyTests, IsGetPropertyConst)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineProperties<std::string> cls(typeLibrary,typeLibrary->CreateType(typeid(std::string), "string"));
    EXPECT_TRUE(std::is_const<std::remove_pointer_t<const std::string*>>::value);
    EXPECT_TRUE(cls.Get("length", &std::string::length).IsConst());
}

TEST(PropertyTests,CreateSetProperty)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineProperties<std::string> cls(typeLibrary,typeLibrary->CreateType(typeid(std::string), "string"));
    cls.Set("size",&std::string::resize);
}

class AttributeHeader
{
public:
    AttributeHeader(const std::string& name) {}
};

TEST(PropertyTests, AddAttributesToProperty)
{
    TypeLibraryPointer typeLibrary = TypeLibraryFactory::CreateDefaultTypeLibrary();
    DefineProperties<std::string> cls(typeLibrary,typeLibrary->CreateType(typeid(std::string), "string"));
    cls.Set("size", &std::string::resize).AddAttribute<AttributeHeader>("Hello world");
    EXPECT_TRUE(typeLibrary->LookupType(typeid(std::string))->Lookup("size")->GetAttributeCollection().HasAttribute<AttributeHeader>());
}
