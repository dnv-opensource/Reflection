//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "Reflection/Objects/Object.h"
#include "Reflection/Objects/Delegate.h"


#include "Reflection/TypeConversions/BuiltInConversions.h"

#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Reflect.h"

#include "Vector3d.h"
#include <list>
#include <memory>


using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Objects;

template<typename Result, typename Input>
Result Query(TypeLibraryPointer lib, const Input& input, const char* what)
{
    return Object(lib, input).Lookup(what)().As<Result>();
}

template<typename InputContainer, typename ResultContainer>
void Query(TypeLibraryPointer lib, const InputContainer& input, const char* what, ResultContainer& result)
{
    typedef typename ResultContainer::value_type value_type;
    for(InputContainer::const_iterator it = input.begin(); it != input.end(); ++it)
    {
        result.insert(result.end(), Query<value_type>(lib, *it, what));
    }
}

TEST(QueryTest, SingleQuery)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<Vector3d>(lib);

    Vector3d v(3, 4, 0);
    EXPECT_DOUBLE_EQ(5, Query<double>(lib, v, "Length"));
}

TEST(QueryTest, ContainerQuery)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<Vector3d>(lib);

    std::list<Vector3d> args;
    args.push_back(Vector3d(3, 4, 0));
    args.push_back(Vector3d(1, 1, 1));
    std::list<double> results;
    Query(lib, args, "X", results);
    EXPECT_DOUBLE_EQ(3, results.front());
    EXPECT_DOUBLE_EQ(1, results.back());
}

TEST(QueryTest, DISABLED_SingleQueryString)
{
    Vector3d v(3, 4, 0);
    //No support for converting arbitrary types to string at this point
    //CPPUNIT_ASSERT_EQUAL(std::string("5.0"),Query<std::string>(v,"Length"));
}
