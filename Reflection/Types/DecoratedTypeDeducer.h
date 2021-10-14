#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>
#include <string>
#include "TypeDecoration.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    /*
    DecoratedTypeDeducer takes any decorated type, such as int**& or const char (&)[2] and
    1. Finds the naked type (int**& -> int, const char (&)[2] -> char)
    2. Finds the decoration (int**& -> **&, const char (&)[2] -> const (&)[2]    
    */
    template<typename T>
    struct DecoratedTypeDeducer 
    {
        //The type without any decoration.
        //const double& -> double
        //std::string* -> std::string
        typedef T Type;
        //The int type with all the decorations.
        //const double& -> const int&
        //std::string* -> int*
        typedef int Decoration;
        //The decorated type.
        //const double& -> const double&
        typedef T DecoratedType;

        //A way to copy decoration from one type to another. Usage:
        //DecoratedTypeDeducer<const double&>::Apply<float>::type type; -> const float&
        template<typename U>
        struct Apply 
        {
            typedef U type;
        };
        //An integer describing the decoration. See TypeDecoration for detail.
        //This integer will simplify arrays into pointers.
        static const int value = TypeDecoration::None;
    };

    template<typename T>
    struct DecoratedTypeDeducer<T&> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_lvalue_reference<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_lvalue_reference<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type& type;
        };
        static const int value = TypeDecoration::Reference + DecoratedTypeDeducer<T>::value;
    };

    template<typename T>
    struct DecoratedTypeDeducer<T*> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type* type;
        };
        static const int value = TypeDecoration::Pointer + DecoratedTypeDeducer<T>::value * TypeDecoration::Offset;
    };

    template<typename T>
    struct DecoratedTypeDeducer<T const> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_const<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_const<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type const type;
        };
        static const int value = TypeDecoration::Const + DecoratedTypeDeducer<T>::value;
    };

    template<typename T>
    struct DecoratedTypeDeducer<T volatile> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_volatile<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_volatile<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type volatile type;
        };
        static const int value = TypeDecoration::Volatile + DecoratedTypeDeducer<T>::value;
    };

    template<typename T>
    struct DecoratedTypeDeducer<T const volatile> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_cv<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_cv<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type const volatile type;
        };
        static const int value = TypeDecoration::Const + TypeDecoration::Volatile + DecoratedTypeDeducer<T>::value;
    };
    template<typename T>
    struct DecoratedTypeDeducer<T[]> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type type[];
        };
        static const int value = TypeDecoration::Pointer + DecoratedTypeDeducer<T>::value*TypeDecoration::Offset;
    };

    template<typename T, int size>
    struct DecoratedTypeDeducer<T[size]> 
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::Decoration>::type Decoration;
        typedef typename std::add_pointer<typename DecoratedTypeDeducer<T>::DecoratedType>::type DecoratedType;
        template<typename U>
        struct Apply 
        {
            typedef typename DecoratedTypeDeducer<T>::template Apply<U>::type type[size];
        };
        static const int value = TypeDecoration::Pointer + DecoratedTypeDeducer<T>::value*TypeDecoration::Offset;
    };

    template<typename T, int size>
    struct DecoratedTypeDeducer<const T[size]>
    {
        typedef typename DecoratedTypeDeducer<T>::Type Type;
        typedef std::add_const_t<std::add_pointer_t<typename DecoratedTypeDeducer<T>::Decoration>> Decoration;
        typedef std::add_const_t<std::add_pointer_t<typename DecoratedTypeDeducer<T>::DecoratedType>> DecoratedType;
        template<typename U>
        struct Apply
        {
            typedef typename DecoratedTypeDeducer<const T>::template Apply<U>::type type[size];
        };
        static const int value = TypeDecoration::Const + TypeDecoration::Pointer + DecoratedTypeDeducer<T>::value*TypeDecoration::Offset;
    };

    /**Changes the decoration of DestType to match the decoration of SourceType.    
    RedecorateType<const int&,double*>::type = const double&
    */
    template<typename SourceType, typename DestType>
    struct RedecorateType 
    {
        typedef typename DecoratedTypeDeducer<DestType>::Type NakedType;
        typedef typename DecoratedTypeDeducer<SourceType>::Apply<NakedType>::type type;
    };

    inline std::string GetDecorationName(int decoration)
    {
        if (decoration == TypeDecoration::ManagedHandle)
            return " ^";
        else if (decoration == TypeDecoration::ManagedReference)
            return " %";
        else if (decoration == TypeDecoration::ManagedHandle + TypeDecoration::ManagedReference)
            return " ^%";

        else if(decoration == TypeDecoration::None)
            return "";
        std::string local;        
        if(decoration&TypeDecoration::Pointer) 
            local  += " *";
        if(decoration&TypeDecoration::Volatile) 
            local  += " volatile";
        if(decoration&TypeDecoration::Const) 
            local  += " const";
        if(decoration&TypeDecoration::Reference) 
            local  += " &";        
        return GetDecorationName(decoration / TypeDecoration::Offset) + local;
    }

}}}}