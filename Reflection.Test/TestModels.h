#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection\TypeLibraries\TypeLibraryPointer.h"
#include "Reflection\Classes\Class.h"

namespace DNVS {namespace MoFa {namespace Reflection {
    class TestBaseClass {
    public:
        virtual ~TestBaseClass() {}
    };
    static void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestBaseClass**)
    {
        using namespace Classes;
        Class<TestBaseClass> cls(typeLibrary, "TestBaseClass");
    }

    class TestDerivedClass : public TestBaseClass 
    {
    };
    static void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TestDerivedClass**)
    {
        using namespace Classes;
        Class<TestDerivedClass,Public<TestBaseClass>> cls(typeLibrary, "TestDerivedClass");
    }

}}}