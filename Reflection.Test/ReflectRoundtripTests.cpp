//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\Variants\VariantService.h"
#include "Reflection\Variants\Variant.h"
#include <memory>
#include "gtest\gtest.h"

#include "GoogleTest\Profiler.h"
#include "TypeUtilities\IntrusiveClass.h"
#include "boost\smart_ptr\intrusive_ptr.hpp"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Variants {
    using namespace Types;
    TEST(ReflectRoundtripTests, ReflectInt_Roundtrip)
    {
        Variant v = VariantService::Reflect(5);
        EXPECT_EQ(5, VariantService::Unreflect<int>(v));
    }

    class Base1 
    {
    public:
        virtual ~Base1() {}
    };
    class Base2 
    {
    public:
        virtual ~Base2() {}
    };
    class Derived : public Base1, public Base2 {};

    TEST(ReflectRoundtripTests, ReflectVirtualClass_Roundtrip)
    {
        Derived d;
        Variant v = VariantService::Reflect(&d);
        EXPECT_EQ(&d, VariantService::Unreflect<Derived*>(v));
    }
    struct MyPodClass 
    {
        int a;
        double b;
        double c;
    };
    MyPodClass GetPodClass(int a, double b, double c) 
    {
        MyPodClass pd;
        pd.a = a;
        pd.b = b;
        pd.c = c;
        return pd;
    }
    TEST(ReflectRoundtripTests, ReflectPodClass)
    {
        MyPodClass pd;
        pd.a = 5;
        pd.b = 42.3;
        pd.c = 19.1;

        Derived d;
        Variant v = VariantService::Reflect(GetPodClass(5, 42.3, 19.1));
        MyPodClass copy = VariantService::Unreflect<const MyPodClass&>(v);
        EXPECT_EQ(pd.a, copy.a);
        EXPECT_EQ(pd.b, copy.b);
        EXPECT_EQ(pd.c, copy.c);
    }

    TEST(ReflectRoundtripTests, ReflectReferenceToVirtualClass_Roundtrip)
    {
        Derived d;
        Variant v = VariantService::Reflect(d);
        EXPECT_EQ(&d, &VariantService::Unreflect<Derived&>(v));
    }

    TEST(ReflectRoundtripTests, ReflectVirtualClass_Roundtrip_FromBase)
    {
        Derived d;
        Base2* b = &d;
        Variant v = VariantService::Reflect(b);
        EXPECT_EQ(b, VariantService::Unreflect<Base2*>(v));
    }

    TEST(ReflectRoundtripTests, ReflectConstPointer_Roundtrip)
    {
        Derived d;
        const Base2* b = &d;
        Variant v = VariantService::Reflect(b);
        EXPECT_EQ(b, VariantService::Unreflect<const Base2*>(v));
    }

    TEST(ReflectRoundtripTests, ReflectConstReference_Roundtrip)
    {
        MyPodClass pd;
        pd.a = 5;
        pd.b = 42.3;
        pd.c = 19.1;
        const MyPodClass& pdref = pd;
        Variant v = VariantService::Reflect(pdref);
        MyPodClass copy = VariantService::Unreflect<const MyPodClass&>(v);
        EXPECT_EQ(pd.a, copy.a);
        EXPECT_EQ(pd.b, copy.b);
        EXPECT_EQ(pd.c, copy.c);
    }

    TEST(ReflectRoundtripTests, ReflectSharedPointer_Roundtrip)
    {
        Variant v = VariantService::Reflect(std::shared_ptr<double>(new double(34.4)));
        EXPECT_NEAR(34.4, *VariantService::Unreflect<std::shared_ptr<double> >(v), 1e-6);
    }

    TEST(ReflectRoundtripTests, ReflectConstRefSharedPointer_Roundtrip)
    {
        Variant v = VariantService::ReflectType<const std::shared_ptr<double>&>(std::shared_ptr<double>(new double(34.4)));
        EXPECT_NEAR(34.4, *VariantService::Unreflect<std::shared_ptr<double> >(v), 1e-6);
        EXPECT_NEAR(34.4, *VariantService::Unreflect<double*>(v), 1e-6);
    }

    TEST(ReflectRoundtripTests, ReflectSharedPointer_GetPointee)
    {
        Variant v = VariantService::Reflect(std::shared_ptr<double>(new double(34.4)));
        EXPECT_NEAR(34.4, *VariantService::Unreflect<double*>(v), 1e-6);
    }

    TEST(ReflectRoundtripTests, ReflectPointer_GetSharedPointer_Fail)
    {
        double x = 34.4;
        Variant v = VariantService::Reflect(&x);
        ASSERT_THROW(VariantService::Unreflect<std::shared_ptr<double> >(v), ConversionError);
    }

    TEST(ReflectRoundtripTests, ReflectRefToSharedPointer_GetPointee)
    {
        std::shared_ptr<double> spr(new double(34.4));
        Variant v = VariantService::Reflect(spr);
        EXPECT_NEAR(34.4, *VariantService::Unreflect<double*>(v), 1e-6);
    }

    TEST(ReflectRoundtripTests, ReflectConstBool)
    {
        const bool a = true;
        Variant v = VariantService::ReflectType<const bool>(a);
        EXPECT_EQ(true, VariantService::Unreflect<const bool>(v));
    }
    class IntrusiveInt : public TypeUtilities::IntrusiveClass<IntrusiveInt>
    {
    public:
        IntrusiveInt(int a) : m_a(a) {}
        operator int() const {return m_a; }
    private:
        int m_a;
    };

    TEST(ReflectRoundtripTests, ReflectBoostIntrusivePtr)
    {
        boost::intrusive_ptr<IntrusiveInt> a(new IntrusiveInt(4));
        Variant v = VariantService::Reflect(a);
        EXPECT_EQ(4, *VariantService::Unreflect<IntrusiveInt*>(v));
        EXPECT_EQ(4, *VariantService::Unreflect<boost::intrusive_ptr<IntrusiveInt>>(v));
    }

    class MyTypeInfo 
    {
    public:
        inline MyTypeInfo() 
            :   m_type(0) 
            ,   m_decoration(-1) 
        {}
        inline MyTypeInfo(const std::type_info& type, int decoration) 
            :   m_type(&type) 
            ,   m_decoration(decoration) 
        {}
        inline bool operator == (const MyTypeInfo& other) const 
        {
            if(m_decoration != other.m_decoration)
                return false;
            if(m_type == other.m_type)
                return true;
            else if(m_type && other.m_type)
                return *m_type == *other.m_type;
            else
                return false;
            return true;
        }
        inline bool operator != (const MyTypeInfo& other) const
        {
            return !operator==(other);
        }
    private:
        const std::type_info* m_type;
        int m_decoration;
    };

    template<typename T>
    const MyTypeInfo& MyTypeId() 
    {
        static MyTypeInfo mt = MyTypeInfo(typeid(T), DecoratedTypeDeducer<T>::value);
        return mt;
    }

    class NewVariant 
    {
    public:
        inline NewVariant() : m_data(0) {}
        inline NewVariant(const MyTypeInfo& type) : m_data(0) , m_type(type) {}
        inline void SetData(void* data) {m_data = data; }
        inline void* GetData() const {return m_data; }
        inline const MyTypeInfo& GetType() const {return m_type; }
    private:
        void* m_data;
        MyTypeInfo m_type;
        void* m_extra;
    };

    struct NewVariantService 
    {
        template<typename T>
        static NewVariant Reflect(T variable)
        {
            NewVariant var(MyTypeId<T>());
            typedef std::remove_cv<T>::type NakedType;
            union 

            {
                NakedType source;
                void* data;
            };
            //Ensure the initial value is NULL
            data = 0;
            //Sets the relevant bits.
            source = variable;
            var.SetData(data);
            return var;
        }
        template<typename T>
        static T Unreflect(const NewVariant& var)
        {
            if(MyTypeId<T>() != var.GetType())
                throw std::runtime_error("");
            typedef std::remove_cv<T>::type NakedType;
            union 

            {
                NakedType dest;
                void* data;
            };
            data = const_cast<void*>(var.GetData());
            return dest;        
        }
    };
    TEST(ReflectionPerformanceWithoutConversion, DISABLED_IsSameOrderOfMagnitudeAsNativeSpeed)
    {
        auto baseline = []() -> int
        {
            volatile int a = 5;
            volatile int b = a;
            return b;
        };
        auto contender = []() -> int
        {
            Variant v = VariantService::ReflectType<int>(5);
            return VariantService::Unreflect<int>(v);
        };
        auto contender2 = []() -> int
        {
            Variant v = VariantService::ReflectType<int>(5);
            return InternalVariantService::UnreflectUnchecked<int>(v);
        };
        auto contender3 = []() -> int
        {        
            NewVariant v = NewVariantService::Reflect<int>(5);
            return NewVariantService::Unreflect<int>(v);
        };

        std::cout << Profile(baseline, contender, 6.0);
    }
}}}}