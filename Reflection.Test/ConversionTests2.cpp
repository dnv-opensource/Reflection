//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest/gtest.h"

#include <Reflection/TypeConversions/ConversionGraph.h>
#include <Reflection/TypeConversions/BuiltInConversions.h>
#include <Reflection/TypeConversions/TypeConversionService.h>
#include <Reflection/TypeConversions/UserConversion.h>

#include "Reflection/TypeConversions/DowncastConversion.h"
#include "Reflection/TypeConversions/InheritanceConversions.h"
#include "Reflection/TypeConversions/PointerReferenceConversion.h"
#include "Reflection/TypeConversions/TrivialConversion.h"
#include "Reflection/TypeConversions/ImplicitConversion.h"

#include "Reflection/Variants/InternalVariantService.h"

#include <Reflection/TypeLibraries/TypeLibraryFactory.h>

#include "TypeUtilities/IntrusiveClass.h"

#include "GoogleTest/Profiler.h"
#include <memory>


namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    using namespace Types;
    using namespace Variants;

    TEST(ConversionTests, POD_Conversion_Fail)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect("42");
        ASSERT_THROW(service.As<int>(v), ConversionError);
    }

    TEST(ConversionTests, POD_Conversion_Success)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(5.6);
        ASSERT_NO_THROW(service.As<int>(v));
    }

    TEST(ConversionTests, Pointer_Conversion_Fail)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(5.6);
        ASSERT_THROW(service.As<const double*>(v), ConversionError);
        ASSERT_THROW(service.As<double*>(v), ConversionError);
    }

    TEST(ConversionTests, Value_Conversion_Fail)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect("4.5");
        ASSERT_THROW(service.As<double>(v), ConversionError);
    }

    TEST(ConversionTests, Conversion_short_to_int)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect<short>(345);
        EXPECT_EQ(345, service.As<int>(v));
    }

    TEST(ConversionTests, Conversion_Int_Is_Double)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(43);
        EXPECT_TRUE(service.IsConvertibleTo<double>(v));
    }

    TEST(ConversionTests, Conversion_Double_Is_Int)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(42.4);
        EXPECT_TRUE(service.IsConvertibleTo<int>(v));
    }

    TEST(ConversionTests, Conversion_Double_Is_RefDouble_Fail)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(42.4);
        EXPECT_TRUE(!service.IsConvertibleTo<double&>(v));
    }

    TEST(ConversionTests, Conversion_Double_Is_CRefDouble)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant v = VariantService().Reflect(42.4);
        EXPECT_TRUE(service.IsConvertibleTo<const double&>(v));
    }

    class ClassWithDoubleAndIntConstructors 
    {
    public:
        ClassWithDoubleAndIntConstructors(double arg) :m_arg(arg) {}
        ClassWithDoubleAndIntConstructors(int arg) : m_arg(arg) {}
    public:
        double m_arg;
    };

    TEST(ConversionTests, UserConversion_doubleToClass)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<double, ClassWithDoubleAndIntConstructors>(conversionGraph);
        AddUserConversion<int, ClassWithDoubleAndIntConstructors>(conversionGraph);

        Variant  v = VariantService().Reflect(5.3);
        EXPECT_TRUE(service.IsConvertibleTo<ClassWithDoubleAndIntConstructors>(v));
        ClassWithDoubleAndIntConstructors cls = service.As<ClassWithDoubleAndIntConstructors>(v);
        EXPECT_NEAR(5.3, cls.m_arg, 1e-6);
    }

    TEST(ConversionTests, UserConversion_intToClass)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<double, ClassWithDoubleAndIntConstructors>(conversionGraph);
        AddUserConversion<int, ClassWithDoubleAndIntConstructors>(conversionGraph);

        Variant v = VariantService().Reflect(42);
        EXPECT_TRUE(service.IsConvertibleTo<const ClassWithDoubleAndIntConstructors&>(v));
        Variant converted = service.GetConversionSequence<const ClassWithDoubleAndIntConstructors&>(v)->Convert(v);
        const ClassWithDoubleAndIntConstructors& cls = service.As<const ClassWithDoubleAndIntConstructors&>(converted);
        EXPECT_NEAR(42, cls.m_arg, 1e-6);    
    }

    class ClassWithStringArg 
    {
    public:
        ClassWithStringArg(const std::string& arg) :m_arg(arg) {}
    public:
        std::string m_arg;
    };

    TEST(ConversionTests, UserConversion_StringToClass)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<const std::string&, ClassWithStringArg>(conversionGraph);

        ///Makes a copy of the string.
        Variant  v = VariantService().Reflect(std::string("Hello world"));
        EXPECT_TRUE(service.IsConvertibleTo<ClassWithStringArg>(v));
        ClassWithStringArg cls = service.As<ClassWithStringArg>(v);
        EXPECT_EQ("Hello world", cls.m_arg);
    }

    TEST(ConversionTests, UserConversion_StringReferenceToClass)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<const std::string&, ClassWithStringArg>(conversionGraph);
        AddTrivialConversions<std::string>(conversionGraph);
        std::string str("Hello world");
        Variant  v = VariantService().Reflect(str);
        EXPECT_TRUE(service.IsConvertibleTo<ClassWithStringArg>(v));
        ClassWithStringArg cls = service.As<ClassWithStringArg>(v);
        EXPECT_EQ("Hello world", cls.m_arg);

    }

    TEST(ConversionTests, IntToCRefInt)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<const std::string&, ClassWithStringArg>(conversionGraph);
        AddTrivialConversions<std::string>(conversionGraph);
        Variant  v = VariantService().Reflect(54);
        EXPECT_TRUE(service.IsConvertibleTo<const int&>(v));
        const int& res = service.IntrusiveAs<const int&>(v);
        EXPECT_EQ(54, res);

    }

    class BaseClass 
    {
    public:
        virtual ~BaseClass() {}
    };

    class DerivedClass : public BaseClass 
    {

    };

    class EvenMoreDerivedClass : public DerivedClass 
    {

    };
    TEST(ConversionTests, RegisterCastToBaseClass)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddImplicitConversion<DerivedClass*, BaseClass*>(conversionGraph);
        TypeConversionService service(conversionGraph);
        DerivedClass cls;
        Variant  v = VariantService().Reflect(&cls);
        EXPECT_TRUE(service.IsConvertibleTo<BaseClass*>(v));
        BaseClass* res = service.As<BaseClass*>(v);
        EXPECT_EQ(&cls, res);
    }

    TEST(ConversionTests, RegisterCastToBaseClass_ReflectBase)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddDowncastConversion<BaseClass, DerivedClass>(conversionGraph);
        TypeConversionService service(conversionGraph);
        DerivedClass cls;
        BaseClass* base = &cls;
        Variant  v = VariantService().Reflect(base);
        EXPECT_TRUE(service.IsConvertibleTo<DerivedClass*>(v));
        DerivedClass* res = service.As<DerivedClass*>(v);
        EXPECT_EQ(&cls, res);
    }

    TEST(ConversionTests, LongDoubleToDouble)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);
        Variant  v = VariantService().Reflect((long double)(4.5));
        EXPECT_TRUE(service.IsConvertibleTo<double>(v));
        double res = service.IntrusiveAs<double>(v);
        EXPECT_NEAR(4.5, res, 1e-6);
    }

    TEST(ConversionTests, IntRefToInt)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<const std::string&, ClassWithStringArg>(conversionGraph);
        AddTrivialConversions<std::string>(conversionGraph);
        int value = 54;
        Variant  v = VariantService().Reflect(value);
        EXPECT_TRUE(service.IsConvertibleTo<int>(v));
        int res = service.As<int>(v);
        EXPECT_EQ(54, res);
    }

    TEST(ConversionTests, DoubleRefToDouble)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddTrivialConversions<std::string>(conversionGraph);
        double value = 54;
        Variant  v = VariantService().Reflect(value);
        EXPECT_TRUE(service.IsConvertibleTo<double>(v));
        double res = service.As<double>(v);
        EXPECT_EQ(54, res);
    }

    TEST(ConversionTests, PointerToConstPointer)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddTrivialConversions<std::string>(conversionGraph);
        double value = 54;
        Variant  v = VariantService().Reflect(&value);
        EXPECT_TRUE(service.IsConvertibleTo<const double*>(v));
        const double* res = service.As<const double*>(v);
        EXPECT_EQ(54, *res);
    }

    TEST(ConversionTests, InheritanceConversions_CastUp)
    {
        ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());
        AddInheritanceConversions<BaseClass, DerivedClass>(conversionGraph);

        EvenMoreDerivedClass evenMoreDerived;
        DerivedClass& derived = evenMoreDerived;
        Variant v = VariantService().Reflect(derived);

        TypeConversionService service(conversionGraph);
        EXPECT_TRUE(service.IsConvertibleTo<BaseClass&>(v));
        ASSERT_NO_THROW(BaseClass& base = service.As<BaseClass&>(v));
    }

    TEST(ConversionTests, InheritanceConversions_CastDown)
    {
        ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());
        AddInheritanceConversions<BaseClass, DerivedClass>(conversionGraph);

        EvenMoreDerivedClass evenMoreDerived;
        BaseClass& base = evenMoreDerived;
        Variant v = VariantService().Reflect(base);

        TypeConversionService service(conversionGraph);
        EXPECT_TRUE(service.IsConvertibleTo<DerivedClass&>(v));
        ASSERT_NO_THROW(DerivedClass& base = service.As<DerivedClass&>(v));
    }

    TEST(ConversionTests, InheritanceConversions_CastDown_ConvertToConstPointer)
    {
        ConversionGraphPointer conversionGraph(
                    TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary()->GetConversionGraph());
        AddInheritanceConversions<BaseClass, DerivedClass>(conversionGraph);
        AddTrivialConversions<BaseClass>(conversionGraph);
        AddTrivialConversions<DerivedClass>(conversionGraph);
        AddPointerReferenceConversions<BaseClass>(conversionGraph);
        AddPointerReferenceConversions<DerivedClass>(conversionGraph);

        EvenMoreDerivedClass evenMoreDerived;
        BaseClass& base = evenMoreDerived;
        Variant v = VariantService().Reflect(base);

        TypeConversionService service(conversionGraph);
        EXPECT_TRUE(service.IsConvertibleTo<const DerivedClass*>(v));
        ASSERT_NO_THROW(const DerivedClass* base = service.As<const DerivedClass*>(v));
    }
    struct StorageClass 
    {
        template<typename T>
        void* AddStorage(T* pointer) 
        {
            return pointer;
        }
    };

    class IStorage : public DNVS::MoFa::TypeUtilities::IntrusiveClass<IStorage>
    {};

    class WithStorage 
    {
    public:
        WithStorage(void* in) : ptr(in) {}
        void* ptr;
        TypeUtilities::IntrusivePointer<IStorage> m_storage;
    private:
    };

    class IConverterClass 
    {
    public:
        virtual void Convert(Variant& input) = 0;
    };
    template<typename From, typename To>
    class ConverterClass : public IConverterClass
    {
    public:
        virtual void Convert(Variant& input) 
        {
            To* to = static_cast<From*>(input.Data());
            input.SetData(static_cast<void*>(to));
        }
    };
    template<typename From, typename To>
    void* Convert(void* var, StorageClass& storage) 
    {
        To* to = static_cast<From*>(var);
        return static_cast<void*>(to);
    }

    TEST(ConversionTests, ConvertPerformanceMeasurements)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        AddUserConversion<const std::string&, ClassWithStringArg>(conversionGraph);
        AddTrivialConversions<std::string>(conversionGraph);

        std::string str("Hello world");
        Variant v = VariantService().Reflect(str);
        EXPECT_TRUE(service.IsConvertibleTo<ClassWithStringArg>(v));
        ConversionSequencePointer conversion = service.GetConversionSequence<ClassWithStringArg>(v);    
        clock_t start = clock();
        for(size_t i = 0; i<10000/*000*/; ++i) {
            ClassWithStringArg arg = service.As<ClassWithStringArg>(v);
        }
        Variant in = v;
        conversion->IntrusiveConvert(in);
        ClassWithStringArg cls = InternalVariantService::UnreflectUnchecked<ClassWithStringArg>(in);
        EXPECT_EQ("Hello world", cls.m_arg);
        clock_t end = clock();
        std::cout << double(end-start) / CLOCKS_PER_SEC << std::endl;
    }
    class MyBase {};
    class MyDerived : public MyBase 
    {
    public:
        MyDerived(int a) {}
    };

    TEST(ConversionTests, PreventUserConversionToUnrelatedType)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddTrivialConversions<MyBase>(conversionGraph);
        AddTrivialConversions<MyDerived>(conversionGraph);
        AddInheritanceConversions<MyBase, MyDerived>(conversionGraph);
        AddUserConversion<int, MyDerived>(conversionGraph);
        Variant v = VariantService().ReflectType<int>(5);
        EXPECT_TRUE(!TypeConversionService(conversionGraph).IsConvertibleTo<const MyBase&>(v));
    }

    class CounterClass : public TypeUtilities::IntrusiveClass<CounterClass>
    {
    public:
        CounterClass(int& counter)
            : m_counter(counter)
        {
            ++m_counter;
        }
        ~CounterClass()
        {
            --m_counter;
        }
    private:
        int& m_counter;
    };
    TEST(ConversionTests, CheckThatClassIsDeletedWhenVariantOfSharedPtrGoesOutOfScope)
    {
        int counter = 0;
        {
            Variant v = VariantService().ReflectType<std::shared_ptr<CounterClass>>(std::make_shared<CounterClass>(counter));
            EXPECT_EQ(1, counter);
        }
        EXPECT_EQ(0, counter);
    }

    TEST(ConversionTests, CheckThatClassIsDeletedWhenVariantOfIntrusivePtrGoesOutOfScope)
    {
        int counter = 0;
        {
            Variant v = VariantService().ReflectType<TypeUtilities::IntrusivePointer<CounterClass>>(TypeUtilities::IntrusivePointer<CounterClass>(new CounterClass(counter)));
            EXPECT_EQ(1, counter);
        }
        EXPECT_EQ(0, counter);
    }

    typedef void (*Converter)(Variant& var);

    void ConvertIntToDouble(Variant& var)
    {
        double from = InternalVariantService::UnreflectUnchecked<double>(var);
        InternalVariantService::SetVariantData<int>(var, (int)from);
    }

    TEST(ReflectionPerformanceWithConversion, DISABLED_IsSameOrderOfMagnitudeAsNativeSpeed)
    {
        ConversionGraphPointer conversionGraph(new ConversionGraph);
        AddBuiltInConversions(conversionGraph);
        TypeConversionService service(conversionGraph);

        auto baseline = []() -> int
        {
            volatile double a = 5.0;
            volatile int b = (int)a;
            return b;
        };
        auto contender = [&]() -> int
        {
            Variant v = VariantService::ReflectType<double>(5.0);
            return service.As<int>(v);
        };
        auto conversionSequence = conversionGraph->GetConversionSequence(TypeId<double>(), TypeId<int>());
        conversionGraph->GetConversionSequence(TypeId<short int>(), TypeId<double>());
        conversionGraph->GetConversionSequence(TypeId<double>(), TypeId<short>());
        conversionGraph->GetConversionSequence(TypeId<const int&>(), TypeId<double>());
        auto contender2 = [&]() -> int
        {
            Variant v = VariantService::ReflectType<double>(5.0);
            conversionSequence->IntrusiveConvert(v);
            v.SetDecoratedTypeInfo(TypeId<int>());
            return InternalVariantService::UnreflectUnchecked<int>(v);
        };
        std::vector<Converter> converters;
        converters.push_back(&ConvertIntToDouble);
        auto contender3 = [&]() -> int
        {
            Variant v = VariantService::ReflectType<double>(5.0);
            for(size_t i = 0; i < converters.size(); ++i)
                converters[i](v);
            v.SetDecoratedTypeInfo(TypeId<int>());
            return InternalVariantService::UnreflectUnchecked<int>(v);
        };

        std::cout << Profile(baseline, contender2, 6.0);
    }
}}}}
