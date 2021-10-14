//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"
#include "Reflection/TypeLibraries/TypeLibraryFactory.h"
#include "Reflection/Enums/Enum.h"
#include "Reflection/Objects/Object.h"
#include "Reflection/Enums/EnumHelper.h"
#include "Reflection/Enums/ReflectionEnumFormatter.h"
#include "Formatting/DefaultFormatterRules.h"
#include "Services/ScopedServiceProvider.h"
#include "EnumOrValue.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums { namespace Tests {
    enum MyColors {
        Red, Green, Blue
    };
    TEST(EnumTests, RegisterEnum_AccessValues)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Enum<MyColors> myColors(typeLibrary, "MyColors");
        myColors.EnumValue("mcRed", Red);
        myColors.EnumValue("mcGreen", Green);
        myColors.EnumValue("mcBlue", Blue);
        MyColors val = Objects::Object(typeLibrary, typeLibrary->LookupType(typeid(MyColors))->GetAllMembers()["mcRed"]->FastInvoke(std::vector<Variants::Variant>(1, Variants::Variant()))).As<MyColors>();
        EXPECT_EQ(Red, val);
    }

    TEST(EnumTests, RegisterEnum_GetEnumValueName)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Enum<MyColors> myColors(typeLibrary, "MyColors");
        myColors.EnumValue("mcRed", Red);
        myColors.EnumValue("mcGreen", Green);
        myColors.EnumValue("mcBlue", Blue);
        EnumHelper helper(typeLibrary, typeid(MyColors));
        EXPECT_EQ("mcGreen", helper.GetEnumValueName(Green));
    }

    TEST(EnumTests, RegisterEnum_GetFormattedEnumValueName)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Enum<MyColors> myColors(typeLibrary, "MyColors");
        myColors.EnumValue("mcRed", Red);
        myColors.EnumValue("mcLightGreen", Green);
        myColors.AddFormatter(TrimStart(2) + InsertSpaceBeforeCapitalLetters());
        myColors.EnumValue("mcBlue", Blue);
        EnumHelper helper(typeLibrary, typeid(MyColors));
        EXPECT_EQ("Light Green", helper.GetFormattedEnumValueName(Green));
    }

    TEST(EnumTests, RegisterEnum_GetAllValues)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Enum<MyColors> myColors(typeLibrary, "MyColors");
        myColors.EnumValue("mcRed", Red);
        myColors.EnumValue("mcLightGreen", Green);
        myColors.EnumValue("mcBlue", Blue);        
        EnumHelper helper(typeLibrary, typeid(MyColors));
        EXPECT_EQ(3, helper.GetAllEnumValues().size());
    }
    enum ValuesWithInternals{
        Manual = 0,
        FromStructure = 1,
        Aisc = 2,
        FromStructure_Aisc = 3,
        FromStructure_Test = 4,
    };
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ValuesWithInternals**)
    {
        Enum<ValuesWithInternals> myEnum(typeLibrary, "ValuesWithInternals");
        myEnum.AddFormatter(TrimStart(2) + InsertSpaceBeforeCapitalLetters());
        myEnum.EnumValue("ssFromStructure", FromStructure);
        myEnum.EnumValue("ssAisc", Aisc);
        myEnum.InternalEnumValue("", Manual).AddFormatter("Manual");
        myEnum.InternalEnumValue("ssFromStructure", FromStructure_Aisc).AddAlias(Aisc);
        myEnum.InternalEnumValue("ssFromStructure", FromStructure_Test);
    }
    TEST(EnumTests, RegisterEnum_WithInternals)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<ValuesWithInternals>(typeLibrary);
        EnumHelper helper(typeLibrary, typeid(ValuesWithInternals));
        EXPECT_EQ(2, helper.GetAllReflectedEnumValues().size());
        EXPECT_EQ(3, helper.GetAllEnumValues().size());
        auto type = typeLibrary->LookupType(typeid(ValuesWithInternals));
        EXPECT_EQ(2, type->GetAttributeCollection().GetAttribute<Attributes::EnumerableAttribute>().EnumerateObjects().size());
    }

    void TestFormatter(const Formatting::DefaultFormatterRules& rules, const char* fromStructureString, const char* aiscString, const char* manualString, const char* fromStructureAiscString)
    {
        using namespace Formatting;
        FormattingService service;
        service.AddFormatter<IFormatterRules>(rules);
        service.AddFormatter<IEnumFormatter>(std::make_shared<Enums::ReflectionEnumFormatter>());
        EXPECT_EQ(fromStructureString, ToString(FromStructure, service));
        EXPECT_EQ(aiscString, ToString(Aisc, service));
        EXPECT_EQ(manualString, ToString(Manual, service));
        EXPECT_EQ(fromStructureAiscString, ToString(FromStructure_Aisc, service));

        auto typeLibrary = Services::ServiceProvider::Instance().TryGetService<TypeLibraries::ITypeLibrary>();
        EXPECT_EQ(fromStructureString, ToString(Objects::Object(typeLibrary,FromStructure), service));
        EXPECT_EQ(aiscString, ToString(Objects::Object(typeLibrary,Aisc), service));
        EXPECT_EQ(manualString, ToString(Objects::Object(typeLibrary, Manual), service));
        EXPECT_EQ(fromStructureAiscString, ToString(Objects::Object(typeLibrary, FromStructure_Aisc), service));
    }
    TEST(EnumTests, TestFormatting)
    {
        Services::ScopedServiceProvider provider;
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        provider.RegisterService<TypeLibraries::ITypeLibrary>(typeLibrary);
        Reflect<ValuesWithInternals>(typeLibrary);
        using namespace Formatting;
        TestFormatter(DefaultFormatterRules(false, IFormatterRules::Exact, true), "ssFromStructure", "ssAisc", "", "ssFromStructure");
        TestFormatter(DefaultFormatterRules(true, IFormatterRules::Exact, true), "ssFromStructure", "ssAisc", "", "(ssFromStructure)");
        TestFormatter(DefaultFormatterRules(true, IFormatterRules::Exact, false), "From Structure", "Aisc", "Manual", "(Aisc)");
        TestFormatter(DefaultFormatterRules(false, IFormatterRules::Exact, false), "From Structure", "Aisc", "Manual", "Aisc");
    }

    TEST(EnumTests, CheckIfIsEnumFromTypeId)
    {
        EXPECT_EQ(std::string("enum "), std::string(typeid(ValuesWithInternals).name()).substr(0, 5));
    }

    TEST(EnumTests, ConvertEnumOrValueToEnum)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<ValuesWithInternals>(typeLibrary);
        typedef EnumOrValue<ValuesWithInternals, Manual, double> EnumOrDouble;
        Reflect<EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Objects::Object o(typeLibrary, EnumOrDouble(FromStructure));
        ASSERT_TRUE(o.IsConvertibleTo<ValuesWithInternals>());
        ASSERT_EQ(FromStructure, o.As<ValuesWithInternals>());
    }

    TEST(EnumTests, ConvertEnumOrValueToDouble)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<ValuesWithInternals>(typeLibrary);
        typedef EnumOrValue<ValuesWithInternals, Manual, double> EnumOrDouble;
        Reflect<EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Objects::Object o(typeLibrary, EnumOrDouble(1.4));
        ASSERT_TRUE(o.IsConvertibleTo<double>());
        ASSERT_EQ(1.4, o.As<double>());
    }

    TEST(EnumTests, ConvertEnumOrValue_InvalidDouble_ToDouble_Throw)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary();
        Reflect<ValuesWithInternals>(typeLibrary);
        typedef EnumOrValue<ValuesWithInternals, Manual, double> EnumOrDouble;
        Reflect<EnumOrDouble>(typeLibrary, "EnumOrDouble");
        Objects::Object o(typeLibrary, EnumOrDouble(FromStructure));
        ASSERT_TRUE(o.IsConvertibleTo<double>());
        EXPECT_THROW(o.As<double>(), std::runtime_error);
    }

}}}}}
