#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <memory>
#include "TypeUtilities/SmartPointerHelper.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Types {   
    /**Checks whether a type is a pod type (no constructors/destructors) and has a size<=void*. 
    IsSmallPodType<int> == true
    IsSmallPodType<double> == true (for 64 bit) and false (for 32 bit)
    struct TestType {
    char a,b;
    };
    IsSmallPodType<TestType> == true
    IsSmallPodType<std::string> == false
    */
    template<typename T>
    struct IsSmallPodType : std::integral_constant<bool,
        !std::is_pointer<T>::value && 
        std::is_pod<T>::value && 
        (sizeof(T)<=sizeof(void*))
    >
    {};

    template<>
    struct IsSmallPodType<void> : std::integral_constant<bool,false> {};

    /**
    Checks whether a type is a smart pointer, such as std::shared_ptr, mofa::ref, DatabasePointer etc.
    Requires an implementation of get_pointer for the smart pointer.
    */
    template<typename T>
    struct IsPointerWrapper : TypeUtilities::is_smart_pointer<T> 
    {};


    ///Unwraps the pointer type from a smart pointer. Default implementation does nothing.
    template<typename T,typename EnableIf=void>
    struct UnwrapPointer {
        typedef T type;
    };

    /**
    Checks whether a smart pointer supports aliasing, i.e. whether you can reuse its smart pointer counter with a different pointer.
    std::shared_ptr and Utility::SmartPointer has these qualities.
    boost::intrusive_ptr and boost::scoped_ptr does not.
    */
    template<typename T>
    struct IsAliasingSmartPointer : TypeUtilities::is_aliasing_smart_pointer<T>
    {};

    ///Unwraps the pointer type from a smart pointer.
    template<typename T>
    struct UnwrapPointer<T,typename std::enable_if<IsPointerWrapper<T>::value>::type> 
    {
        static T MakeInstance();
        typedef decltype(get_pointer(MakeInstance())) type;
    };

}}}}