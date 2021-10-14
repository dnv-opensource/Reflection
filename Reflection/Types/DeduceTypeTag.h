#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeTraits.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Types {   
    /**
    The Reflection library can support most regular C++ types. The way in which these types are handled, however, differs.
    Therefore all types are categorized and sometimes simplified so that we can provide a unified way of handling them.
    Tags are created as a type hierarchy, with the most generic AnyTag being the base class of all. 
    You can then overload functions on the tag type to allow specializing your code for that particular type.
    The way you can use these tags is to create a function, such as 
    
    
    //Fallback method.
    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(AnyTag) {...}
    
    //Special implementation for types with the ValueTag.
    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(ValueTag) {...}
    
    //Special implementation for types with the SmartPointerTag.
    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(SmartPointerTag) {...}

    template<typename T>
    const DecoratedTypeInfo& TypeId()
    {
       //Extract the tag from the type
       TypeTag<T> tag;
       //DeducedType<T> represents a simplified representation of T.
       //If T == T*&, then DeducedType<T> == T*
       //If T == const std::shared_ptr<int> then DeducedType<T> == std::shared_ptr<int>
       return TypeIdImpl<DeducedType<T>,tag);
    }
    Below is a list of all the tags implemented:
    */
    //Fallback tag.
    struct AnyTag {};
    /**
    Any large class or struct and small classes with nontrivial constructors and/or destructors.
    Examples:
    - std::string
    - pos3d
    - Space::Vector3D.
    */
    struct ValueTag : public AnyTag {};
    
    //void.
    struct VoidTag : public AnyTag {};
    
    /**
    This tag is given to any type that is small enough to fit inside a void*, 
    Examples:
    - int
    - double (on 64 bit)
    - enums
    - small simple structs,
    Not included:
    - pointers
    - references 
    - larger structs 
    - classes with custom constructors or destructors.
    */
    struct SmallPodTag : public AnyTag {};
    
    /**
    This tag is given to any smart pointer
    Examples:
    - std::shared_ptr<double>
    - const std::shared_ptr<int>&
    - boost::intrusive_ptr<std::string>
    - std::unique_ptr<std::string>&&
    - Utility::SmartPointer<scBeam>
    */
    struct SmartPointerTag : public AnyTag {};

    /*This tag is given to any copyable smart pointer.
    Examples:
    - std::shared_ptr<double>
    - const std::shared_ptr<int>&
    - Utility::SmartPointer<scBeam>
    - boost::intrusive_ptr<std::string>
    Not included:
    - std::unique_ptr<std::string>&&
    */   
    struct CopyableSmartPointerTag : public SmartPointerTag {};

    /*This tag is given to any movable smart pointer.
    Examples:
    - std::unique_ptr<std::string>&&
    - std::unique_ptr<std::string>
    Not included:
    - std::shared_ptr<double>
    - const std::shared_ptr<int>&
    - Utility::SmartPointer<scBeam>
    */
    struct MovableSmartPointerTag : public SmartPointerTag {};

    /*This tag is given to any aliasing smart pointer, meaning a smart pointer where reconstruction of the pointer can be done
    from a void pointer + a void version of the smart pointer
    Examples:
    - std::shared_ptr<std::string>
    - Utility::SmartPointer<ICurveOffset>
    Not included:
    - boost::intrusive_ptr<double>
    - std::unique_ptr<int>
    */
    struct AliasingSmartPointerTag : public CopyableSmartPointerTag {};

    /*This tag is given to any pointers
    Examples:
    - int*
    - double*& -> simplified to double*
    - const char (&)[15] -> simplified to const char*
    - int[12] -> simplified to int*
    Not included
    - int[]
    - const char (&)[]
    */
    struct PointerTag : public AnyTag {};

    /*This tag is given to any reference (except references to smart pointers and pointers)
    Examples:
    - int&
    - const double& -> simplified to double*
    Not included
    - int*&
    - const std::shared_ptr<int>&
    */
    struct ReferenceTag : public AnyTag {};

    template<typename T,typename Enable = void>
    struct DeduceTypeTag
    {
        typedef T Type;
        typedef const T& ArgumentType;
        typedef T ReturnType;
        typedef ValueTag Tag;
    };

    template<>
    struct DeduceTypeTag<void,void>
    {
        typedef void Type;
        typedef VoidTag Tag;
    };

    //Detect small pods
    template<typename T>
    struct DeduceTypeTag<T, std::enable_if_t<!std::is_pointer_v<T> && std::is_pod_v<T> && (sizeof(T) <= sizeof(void*))>>
    {
        typedef T Type;
        typedef T ArgumentType;
        typedef T ReturnType;
        typedef SmallPodTag Tag;
    };

    //Detect copyable smart pointers (such as std::shared_ptr) also handles rvalues of these.
    template<typename T>
    struct DeduceTypeTag<T, std::enable_if_t<IsPointerWrapper<T>::value && !IsAliasingSmartPointer<T>::value && std::is_copy_constructible_v<T>>>
    {
        typedef T Type;
        typedef const T& ArgumentType;
        typedef T ReturnType;
        typedef typename UnwrapPointer<T>::type PointerType;
        typedef CopyableSmartPointerTag Tag;
    };

    template<typename T>
    struct DeduceTypeTag<T, std::enable_if_t<IsAliasingSmartPointer<T>::value>>
    {
        typedef T Type;
        typedef const T& ArgumentType;
        typedef T ReturnType;
        typedef typename UnwrapPointer<T>::type PointerType;
        typedef AliasingSmartPointerTag Tag;
    };

    //Detect movable smart pointers (std::unique_ptr)
    template<typename T>
    struct DeduceTypeTag<T, std::enable_if_t<IsPointerWrapper<T>::value && !std::is_copy_constructible_v<T>>>
    {
        typedef T Type;
        typedef T&& ArgumentType;
        typedef T& ReturnType;
        typedef typename UnwrapPointer<T>::type PointerType;
        typedef MovableSmartPointerTag Tag;
    };
    //Detect const references to smart pointers
    template<typename T>
    struct DeduceTypeTag<const T&, std::enable_if_t<IsPointerWrapper<T>::value>>
        : DeduceTypeTag<T>
    {
    };

    //Detect references to smart pointers
    template<typename T>
    struct DeduceTypeTag<T&, std::enable_if_t<IsPointerWrapper<T>::value && !std::is_const_v<T>>>
        : DeduceTypeTag<T>
    {
    };

    //Detect pointers
    template<typename T>
    struct DeduceTypeTag<T*, void>
    {
        typedef T* Type;
        typedef T* ArgumentType;
        typedef T* ReturnType;
        typedef T NakedType;
        typedef PointerTag Tag;
    };

    //References to pointers are not supported, they are treated as pointers.
    template<typename T>
    struct DeduceTypeTag<T*&, std::enable_if_t<!IsPointerWrapper<T>::value>>
        : DeduceTypeTag<T*>
    {
    };

    template<typename T>
    struct DeduceTypeTag<T*const &, std::enable_if_t<!IsPointerWrapper<T>::value>>
        : DeduceTypeTag<T*>
    {
    };

    //Arrays are converted to pointers.
    template<typename T, size_t count>
    struct DeduceTypeTag<T[count]>
        : DeduceTypeTag<T*>
    {
    };
    
    //References to arrays are converted to pointers.
    template<typename T, size_t count>
    struct DeduceTypeTag<T(&)[count]>
        : DeduceTypeTag<T*>
    {
    };

    //Detect references
    template<typename T>
    struct DeduceTypeTag<T&, std::enable_if_t<!IsPointerWrapper<T>::value && !std::is_pointer_v<T> && !std::is_array_v<T>>>
    {
        typedef T& Type;
        typedef T& ArgumentType;
        typedef T& ReturnType;
        typedef T NakedType;
        typedef ReferenceTag Tag;
    };

    template<typename T>
    using TypeTag = typename DeduceTypeTag<T>::Tag;

    template<typename T>
    using ArgumentType = typename DeduceTypeTag<T>::ArgumentType;

    template<typename T>
    using ReturnType = typename DeduceTypeTag<T>::ReturnType;

    template<typename T>
    using DeducedType = typename DeduceTypeTag<T>::Type;
}}}}