#include "gtest/gtest.h"
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ManualConversionGraph.h"
#include "Reflection/TypeConversions/ImplicitConversion.h"
#include "Reflection/Variants/VariantService.h"
#include "Reflection/TypeConversions/BuiltInConversions.h"
#include "Reflection/TypeConversions/TypeConversionService.h"

#include "FallbackDummyConverter.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    TEST(ManualConversionGraphTests, CreateVertex)
    {
        Details::GraphVertex vtxLong(Types::TypeId<long>());
        Details::GraphVertex vtxInt(Types::TypeId<int>());
        vtxLong.AddConversion(&vtxInt, ConversionType::StandardConversion, ConversionPointer());
        ASSERT_EQ(1, vtxLong.GetOutEdges().size());        
        EXPECT_EQ(ConversionType::StandardConversion, vtxLong.GetOutEdges().begin()->GetConversionType());
        vtxLong.AddConversion(&vtxInt, ConversionType::TrivialConversion, ConversionPointer());
        EXPECT_EQ(ConversionType::TrivialConversion, vtxLong.GetOutEdges().begin()->GetConversionType());
        vtxLong.AddConversion(&vtxInt, ConversionType::StandardConversion, ConversionPointer());
        EXPECT_EQ(ConversionType::TrivialConversion, vtxLong.GetOutEdges().begin()->GetConversionType());
    }
    
    TEST(ManualConversionGraphTests, CreateManualConversionGraph_AddConversion)
    {
        ConversionGraphPointer conversionGraph(new ManualConversionGraph);
        AddImplicitConversion<long, double>(conversionGraph);
        ConversionSequencePointer sequence = conversionGraph->GetConversionSequence(Types::TypeId<long>(), Types::TypeId<double>());
        ASSERT_NE(sequence, nullptr);
        ASSERT_TRUE(sequence->IsValid());
        Variants::Variant v = Variants::VariantService::ReflectType<long>(5);
        Variants::Variant result = sequence->Convert(v);
        ASSERT_DOUBLE_EQ(5, Variants::VariantService::Unreflect<double>(result));
    }

    TEST(ManualConversionGraphTests, TestFallbackConversion)
    {
        ConversionGraphPointer conversionGraph(new ManualConversionGraph);
        AddImplicitConversion<double, int>(conversionGraph);
        AddImplicitConversion<float, int>(conversionGraph);
        conversionGraph->AddConversion(Types::TypeId<int>(), Types::TypeId<void>(), ConversionType::FallbackConversion, ConversionPointer(new FallbackDummyConverter));
        EXPECT_FALSE(conversionGraph->GetConversionSequence(Variants::VariantService::ReflectType<int>(5), Types::TypeId<double>())->IsValid());
        EXPECT_TRUE(conversionGraph->GetConversionSequence(Variants::VariantService::ReflectType<int>(5), Types::TypeId<float>())->IsValid());
        EXPECT_FALSE(conversionGraph->GetConversionSequence(Variants::VariantService::ReflectType<int>(-5), Types::TypeId<float>())->IsValid());
        EXPECT_TRUE(conversionGraph->GetConversionSequence(Variants::VariantService::ReflectType<int>(-5), Types::TypeId<double>())->IsValid());
    }


    TEST(ManualConversionGraphTests, TestNoConversionNeeded)
    {
        ConversionGraphPointer conversionGraph(new ManualConversionGraph);
        ConversionSequencePointer sequence = conversionGraph->GetConversionSequence(Types::TypeId<long>(), Types::TypeId<long>());
        ASSERT_NE(nullptr, sequence);
        EXPECT_TRUE(sequence->IsValid());
    }

    TEST(ManualConversionGraphTests, TestDoubleToConstDoubleRef)
    {
        ConversionGraphPointer conversionGraph(new ManualConversionGraph);
        TypeConversions::AddBuiltInConversions(conversionGraph);
        TypeConversions::TypeConversionService service(conversionGraph);
        Variants::Variant myDouble = Variants::VariantService::ReflectType<double>(1.4);
        double crefDouble = service.IntrusiveAs<const double&>(myDouble);
    }

}}}}
