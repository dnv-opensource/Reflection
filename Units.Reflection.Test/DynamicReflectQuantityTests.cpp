//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include "Units/Runtime/DynamicQuantity.h"
#include "Units/Runtime/Unit.h"
#include "Units/Length.h"
#include "Units/Mass.h"
#include "Units/ForAllDimensions.h"
#include "Units/Reflection/ReflectQuantities.h"

#include "Reflection/Classes/Class.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Objects/Object.h"
#include "Units/Reflection/Details/ReflectLength.h"


using namespace DNVS::MoFa::Reflection::Classes;
using namespace DNVS::MoFa::Reflection::Objects;
using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::TypeConversions;
using namespace DNVS::MoFa::Reflection;
using namespace DNVS::MoFa::Units;
using namespace DNVS::MoFa::Units::Runtime;


struct ConverterFromDynamicQuantityToStaticQuantity
{
public:
    ConverterFromDynamicQuantityToStaticQuantity(const Variants::Variant& dynamicQuantity)
        :   m_dynamicQuantity(Variants::VariantService::Unreflect<DynamicQuantity>(dynamicQuantity))
        ,   m_staticQuantity(dynamicQuantity)
    {
    }
    template<typename DimensionT>
    void Apply()
    {
        typedef Quantity<DimensionT> StaticQuantity;
        if(DynamicDimension(DimensionT()) == m_dynamicQuantity.GetSimplifiedUnit().GetDimension())   
            m_staticQuantity = Variants::VariantService::Reflect<StaticQuantity>(StaticQuantity(m_dynamicQuantity.GetNeutralValue()));
    }
    Variants::Variant GetStaticQuantity() const
    {
        return m_staticQuantity;
    }
private:
    Variants::Variant m_staticQuantity;
    DynamicQuantity m_dynamicQuantity;
};

struct FallbackUnitConverter : public IConversion
{
    virtual Variants::Variant Convert(const Variants::Variant& other)
    {
        ConverterFromDynamicQuantityToStaticQuantity converter(other);
        ForAllUsedDimensions(converter);
        return converter.GetStaticQuantity();
    }
    virtual void IntrusiveConvert( Variants::Variant& variable ) 
    {
        ConverterFromDynamicQuantityToStaticQuantity converter(variable);
        ForAllUsedDimensions(converter);
        variable = converter.GetStaticQuantity();
    }
};

TEST(DynamicReflectQuantityTests, ReflectDynamicQuantities_AddLengths)
{
    ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());
    TypeLibraryPointer typeLibrary(new TypeLibrary(conversionGraph));
    Reflection::ReflectDynamicQuantities(typeLibrary);
    Reflection::ReflectLength(typeLibrary);
    Object a(typeLibrary, DynamicQuantity(5.2, _m));
    Object b(typeLibrary, DynamicQuantity(4.2, _m));
    Object c = a + b;
    EXPECT_EQ(DynamicQuantity(5.2 + 4.2, _m), c.As<DynamicQuantity>());
}

TEST(DynamicReflectQuantityTests, ReflectDynamicQuantities_ConvertToLength)
{
    ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());

    TypeLibraryPointer typeLibrary(new TypeLibrary(conversionGraph));
    Reflection::ReflectDynamicQuantities(typeLibrary);
    Reflection::ReflectLength(typeLibrary);
    Object a(typeLibrary, DynamicQuantity(5.2, _m));
    Object b(typeLibrary, DynamicQuantity(4.2, _m));
    Object c = a + b;
    EXPECT_EQ(Length(5.2 + 4.2), c.As<Length>());
    EXPECT_THROW(c.As<Mass>(), std::runtime_error);
}

TEST(DynamicReflectQuantityTests, TestInvalidConversionFromDynamicQuantity_NoCrash)
{
    ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());

    TypeLibraryPointer typeLibrary(new TypeLibrary(conversionGraph));
    Reflection::ReflectDynamicQuantities(typeLibrary);
    Reflection::ReflectLength(typeLibrary);
    Object a(typeLibrary, DynamicQuantity(5.2, Unit("DummyUnit", 1.0, DynamicDimension(4, 4, 4, 4, 4))));
    EXPECT_NO_FATAL_FAILURE(
        EXPECT_THROW(a.As<Length>(), std::runtime_error)
    );
}

TEST(DynamicReflectQuantityTests, TestConversionOfNonDimensionalToDouble)
{
    ConversionGraphPointer conversionGraph(
        TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());

    TypeLibraryPointer typeLibrary(new TypeLibrary(conversionGraph));
    Reflection::ReflectDynamicQuantities(typeLibrary);
    Object a(typeLibrary, DynamicQuantity(5.2, Unit("DummyUnit", 1.0, DynamicDimension(0, 0, 0, 0, 0))));
    EXPECT_DOUBLE_EQ(5.2, a.As<double>());
}
