#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
namespace DNVS { namespace MoFa { namespace Reflection { 
    /**
    For all classes that are to support reflection, you need to implement a DoReflect method.
    Given a type MyType, you need to pass MyType** as the last argument to this function.
    This is why:
    - The method is implemented in the namespace of MyType, not in the namespace of the reflection.
      We need to pass an argument of MyType in order to allow argument dependent lookup (ADL) to find this method and add it to the overload set.
      See also: http://en.cppreference.com/w/cpp/language/adl
    - The reason for the double pointer (**) syntax is that if we have class MyType : public BaseClass {};
      and have implemented DoRegister for BaseClass, we don't want Register<MyType>(typeLibrary) to accidentally
      call the DoRegster for BaseClass. Specifying double pointer prevents all implicit down casting of the pointer.

    You never call DoReflect directly, in stead you call Reflect<MyType>(typeLibrary);
    */
    template<typename... Args>
    void DoReflect(Args... args)
    {
        static_assert(false,
            "You need to implement a DoReflect function for your type.\n"
            "It needs to look like this:\n"
            "void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, YourTypeGoesHere**)\n"
            "{\n"
            "    ...\n"
            "}\n"
            "This method MUST lie in the same namespace as YourTypeGoesHere, otherwise it will not be detected when calling Reflect<YourTypeGoesHere>(...);\n"
            "If you have supplied a custom allocator in the Class descriptor:\n"
            "Class<YourTypeGoesHere, MyCustomAllocator<YourTypeGoesHere>> cls(...);\n"
            "You need to change the signature of DoReflect to this:\n"
            "void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, const MyCustomAllocator<YourTypeGoesHere>& allocator, YourTypeGoesHere**)\n"
        );
    };
}}}
