//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "Reflection/Members/GlobalType.h"

#include "Reflection/Objects/Object.h"
#include "Reflection/Reflect.h"
#include "Reflection/Objects/Delegate.h"

#include "Reflection/TypeConversions/BuiltInConversions.h"

#include "Reflection/TypeLibraries/TypeLibraryFactory.h"


#include "Vector3d.h"
#include <string>
#include <boost/mpl/lambda.hpp>
#include <memory>
#include "Reflection/Attributes/ExpressionExpanderAttribute.h"
#include "Reflection/IExpressionEvaluator.h"
#include "boost/lexical_cast.hpp"
#include "FakeExpressionEvaluator.h"

using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Objects;

TEST(VectorTest,CrossProduct)
{
	TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary();
	AddBuiltInConversions(lib->GetConversionGraph());
	Reflect<Vector3d>(lib);
	Object wrapper(lib,Members::GlobalType());

    Vector3d u(10,0,1);
    Vector3d v(0.5,15.5,-4);
    Vector3d w=u.Cross(v);
    Vector3d x=u*w;

    Object u_reflect(lib,wrapper.Lookup("Vector3d")(10,0,1));
    Object v_reflect(lib,wrapper.Lookup("Vector3d")(0.5,15.5,-4));
    Object w_reflect(u_reflect.Lookup("Cross")(v_reflect));
    Object x_reflect(u_reflect*w_reflect);

    EXPECT_EQ(u,u_reflect.As<const Vector3d&>());
    EXPECT_EQ(v,v_reflect.As<const Vector3d&>());
    EXPECT_EQ(w,w_reflect.As<const Vector3d&>());
    EXPECT_EQ(x,x_reflect.As<const Vector3d&>());

    EXPECT_EQ(Object(lib,u),u_reflect);
    EXPECT_EQ(Object(lib,v),v_reflect);
    EXPECT_EQ(Object(lib,w),w_reflect);
    EXPECT_EQ(Object(lib,x),x_reflect);

    EXPECT_NE(Object(lib,x),v_reflect);
}


TEST(VectorTest, ParseShortSyntax)
{
    TypeLibraryPointer lib = TypeLibraryFactory::CreateDefaultTypeLibrary(false);
    AddBuiltInConversions(lib->GetConversionGraph());
    Reflect<Vector3d>(lib);

    Vector3d baseline(10, 0, 1);

    auto expressionResult = Attributes::ExpandExpression("10 0 1", DNVS::MoFa::Reflection::Types::TypeId<Vector3d>(), FakeExpressionEvaluator(lib));
    EXPECT_TRUE(expressionResult.IsValid());
    EXPECT_EQ("Vector3d(10, 0, 1)", expressionResult.GetString());
}
