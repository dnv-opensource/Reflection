//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <gtest/gtest.h>
#include <Reflection/TypeConversions/FullConversionGraph.h>
#include <Reflection/TypeConversions/ImplicitConversion.h>
#include <Reflection/TypeConversions/TypeConversionService.h>
#include <Reflection/TypeConversions/BuiltInConversions.h>
#include <Reflection/TypeConversions/UserConversion.h>
#include <Reflection/TypeConversions/DynamicTypeDeduction.h>
#include <Reflection/Types/DecoratedTypeInfo.h>
#include <Reflection/Variants/Variant.h>
#include <Reflection/Variants/VariantService.h>

#include "FallbackDummyConverter.h"

using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection::Types;
using namespace DNVS::MoFa::Reflection::Variants;

class FullConversionGraphTests : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_conversionGraph.reset(new FullConversionGraph);
    }

    ConversionGraphPointer m_conversionGraph;
};

TEST_F(FullConversionGraphTests, WhenConversionIsAddedCorrectConversionSequenceIsReturned)
{
    AddImplicitConversion<long, double>(m_conversionGraph);
    ConversionSequencePointer sequence = m_conversionGraph->GetConversionSequence(TypeId<long>(), TypeId<double>());
    ASSERT_NE(sequence, nullptr);
    ASSERT_TRUE(sequence->IsValid());
    Variant v = VariantService::ReflectType<long>(5);
    Variant result = sequence->Convert(v);
    ASSERT_DOUBLE_EQ(5, VariantService::Unreflect<double>(result));
}

TEST_F(FullConversionGraphTests, TestFallbackConversion)
{
    AddImplicitConversion<double, int>(m_conversionGraph);
    AddImplicitConversion<float, int>(m_conversionGraph);
    m_conversionGraph->AddConversion(TypeId<int>(), TypeId<void>(), ConversionType::FallbackConversion, ConversionPointer(new FallbackDummyConverter));
    EXPECT_FALSE(m_conversionGraph->GetConversionSequence(VariantService::ReflectType<int>(5), TypeId<double>())->IsValid());
    EXPECT_TRUE(m_conversionGraph->GetConversionSequence(VariantService::ReflectType<int>(5), TypeId<float>())->IsValid());
    EXPECT_FALSE(m_conversionGraph->GetConversionSequence(VariantService::ReflectType<int>(-5), TypeId<float>())->IsValid());
    EXPECT_TRUE(m_conversionGraph->GetConversionSequence(VariantService::ReflectType<int>(-5), TypeId<double>())->IsValid());
}


TEST_F(FullConversionGraphTests, TestNoConversionNeeded)
{
    ConversionSequencePointer sequence = m_conversionGraph->GetConversionSequence(TypeId<long>(), TypeId<long>());
    ASSERT_NE(nullptr, sequence);
    EXPECT_TRUE(sequence->IsValid());
}

TEST_F(FullConversionGraphTests, TestDoubleToConstDoubleRef)
{
    AddBuiltInConversions(m_conversionGraph);
    TypeConversionService service(m_conversionGraph);
    Variant myDouble = VariantService::ReflectType<double>(1.4);
    double crefDouble = service.IntrusiveAs<const double&>(myDouble);
}

class DummyClass
{
public:
    DummyClass()
    {
    }

    virtual void DummyFunction()
    {
    }
};

class DummyChildClass : public DummyClass
{
public:
    DummyChildClass()
    {
    }

    void DummyFunction() override
    {
    }
};

class AnotherDummyClass
{
public:
    AnotherDummyClass()
    {
    }
};

// DummyClass* pointing to DummyChildClass. Dynamic deduction returns DummyChildClass but there is no conversion
// DummyChildClass -> AnotherDummyClass. There is however a conversion DummyClass -> AnotherDummyClass and it should
// be returned.
TEST_F(FullConversionGraphTests, WhenDynamicConversionReturnsInvalidConversionReturnValidConversion)
{
    AddUserConversion<DummyClass*, AnotherDummyClass>(m_conversionGraph,
    [] (const DummyClass*) -> AnotherDummyClass
    {
        return AnotherDummyClass();
    });
    AddDynamicTypeDeduction<DummyClass>(m_conversionGraph);

    DummyChildClass dummy;

    ASSERT_TRUE(m_conversionGraph->GetConversionSequence(VariantService::ReflectType<DummyClass*>(&dummy),
                                                         TypeId<AnotherDummyClass>())->IsValid());
}
