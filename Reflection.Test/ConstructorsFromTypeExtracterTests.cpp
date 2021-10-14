//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "gtest\gtest.h"
//#include "Utility\SmartPointer.h"
#include "Reflection\TypeLibraries\ITypeLibrary.h"
#include "Reflection\Classes\Class.h"
#include "Reflection\Attributes\ListAsConstructorAttribute.h"
#include "Reflection\Attributes\CaptionAttribute.h"
#include "Reflection\Attributes\DefaultConstructorSelectorAttribute.h"
#include "Reflection\Utilities\ConstructorsFromTypeExtracter.h"
#include "Reflection\TypeLibraries\TypeLibraryFactory.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities { namespace Test {

    class ICurveOffset {
    public:
        virtual ~ICurveOffset() {}
    };
    typedef std::shared_ptr<ICurveOffset> CurveOffsetPointer;

    CurveOffsetPointer NoCurveOffset()
    {
        return CurveOffsetPointer();
    }

    void DoReflect(const TypeLibraries::TypeLibraryPointer& typeLibrary, ICurveOffset**)
    {
        using namespace Reflection::Classes;
        Class<ICurveOffset, std::shared_ptr<ICurveOffset> > cls(typeLibrary, "CurveOffset");

        //Constructors
        cls.NamedConstructor("NoCurveOffset", &NoCurveOffset)
            .AddAttribute<ListAsConstructorAttribute>(ListAsConstructorAttribute::ListTopLevel)
            .AddAttribute<CaptionAttribute>("No curve offset")
            .AddAttribute<DefaultConstructorSelectorAttribute>([](CurveOffsetPointer curveOffset) {return curveOffset == nullptr; })
            ;
    }

    class ConstantCurveOffset : public ICurveOffset
    {
    public:
        ConstantCurveOffset(double x) {}
        double GetX() const { return 1; }
    };
    typedef std::shared_ptr<ConstantCurveOffset> ConstantCurveOffsetPointer;

    void DoReflect(const TypeLibraries::TypeLibraryPointer& typeLibrary, ConstantCurveOffset**)
    {
        using namespace Reflection::Classes;
        Class<ConstantCurveOffset, Public<ICurveOffset>, ConstantCurveOffsetPointer> cls(typeLibrary, "ConstantCurveOffset");
        cls.Constructor<double>(Explicit)
            .AddSignature("x")
            .AddAttribute<CaptionAttribute>("Constant offset")
            ;

        cls.Get("X", &ConstantCurveOffset::GetX)
            ;
    }

    TEST(ConstructorsFromTypeExtracterTests, GetAllConstructors)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstantCurveOffset>(typeLibrary);
        EXPECT_EQ(2, ConstructorsFromTypeExtracter(typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(Types::TypeId<CurveOffsetPointer>(), true).size());
        EXPECT_EQ(1, ConstructorsFromTypeExtracter(typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(Types::TypeId<CurveOffsetPointer>(), false).size());
    }

    TEST(ConstructorsFromTypeExtracterTests, SelectDerivedConstructorFromType)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstantCurveOffset>(typeLibrary);
        auto members = ConstructorsFromTypeExtracter(typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(Types::TypeId<CurveOffsetPointer>(), true);
        auto member = ConstructorsFromTypeExtracter(typeLibrary).GetBestMemberFromType(members, Types::TypeId<ConstantCurveOffsetPointer>());
        ASSERT_NE(nullptr, member);
        EXPECT_EQ(Types::TypeId<ConstantCurveOffsetPointer>(), member->GetReturnType());
    }

    TEST(ConstructorsFromTypeExtracterTests, SelectBaseConstructorFromType)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstantCurveOffset>(typeLibrary);
        auto members = ConstructorsFromTypeExtracter(typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(Types::TypeId<CurveOffsetPointer>(), true);
        auto member = ConstructorsFromTypeExtracter(typeLibrary).GetBestMemberFromType(members, Types::TypeId<CurveOffsetPointer>());
        ASSERT_NE(nullptr, member);
        EXPECT_EQ(Types::TypeId<CurveOffsetPointer>(), member->GetReturnType());
    }

    class FakeCurveOffset : public ICurveOffset
    {
    public:
        FakeCurveOffset(double x) {}
        FakeCurveOffset(double x, double y) {}
        double GetX() const { return 1; }
    };
    typedef std::shared_ptr<FakeCurveOffset> FakeCurveOffsetPointer;

    void DoReflect(const TypeLibraries::TypeLibraryPointer& typeLibrary, FakeCurveOffset**)
    {
        using namespace Reflection::Classes;
        Class<FakeCurveOffset, Public<ICurveOffset>, FakeCurveOffsetPointer> cls(typeLibrary, "FakeCurveOffset");
        cls.Constructor<double>(Explicit)
            .AddSignature("x")
            ;

        cls.Constructor<double, double>(Explicit)
            .AddSignature("x", "y")
            ;

        cls.Get("X", &ConstantCurveOffset::GetX)
            ;
    }

    TEST(ConstructorsFromTypeExtracterTests, SelectBestDerivedConstructorFromType)
    {
        TypeLibraries::TypeLibraryPointer typeLibrary = TypeLibraries::TypeLibraryFactory::CreateDefaultTypeLibrary(false);
        Reflect<ConstantCurveOffset>(typeLibrary);
        Reflect<FakeCurveOffset>(typeLibrary);
        auto members = ConstructorsFromTypeExtracter(typeLibrary).CollectAllConstructorMembersOfThisAndDerivedClasses(Types::TypeId<CurveOffsetPointer>(), true);
        auto member = ConstructorsFromTypeExtracter(typeLibrary).GetBestMemberFromType(members, Types::TypeId<FakeCurveOffsetPointer>());
        ASSERT_NE(nullptr, member);
        EXPECT_EQ(Types::TypeId<FakeCurveOffsetPointer>(), member->GetReturnType());
    }

}}}}}