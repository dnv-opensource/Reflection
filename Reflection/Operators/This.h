#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Operators/Tags.h"
#include "Operators/Invoker.h"
#include "Reflection/Members/OperatorNames.h"
#include "Reflection/Types/TypeTraits.h"
namespace DNVS { namespace MoFa { namespace Reflection { namespace Operators {
    
    template<typename Lhs,typename Rhs,typename Tag, typename Callback>
    class BinaryOperator;
    
    
    template<typename Subject,typename Tag, typename Callback>
    class UnaryOperator;
    
    //Hoist 'This' into special namespace to support Argument dependent lookup:
    //http://en.cppreference.com/w/cpp/language/adl
    //The actual operator overloads used is defined in UnaryOperator.h and BinaryOperator.h
    namespace ThisOperators {
        using namespace DNVS::MoFa::Operators;
        template<typename Lhs,typename Rhs,typename Tag>
        struct BinaryOperatorGenerator;

        enum ThisDecoration {
            ThisType,// e.g. ICurveOffset&
            ThisConstType, //e.g. const ICurveOffset&
            ThisPointerType, //e.g. ICurveOffset*
            ThisConstPointerType, //e.g. const ICurveOffset*
            ThisStorageType, //e.g. Utility::SmartPointer<ICurveOffset>
            ThisConstStorageType, //e.g. const Utility::SmartPointer<ICurveOffset>&
        };
        /*
        'This' is a keyword used when registering operators.
        If you want to represent the operator: const std::string& + const std::string&
        You need to write:
        Class<std::string> cls(...);
        cls.Operator(This.Const + This.Const);
        This will encapsulate the correct operator.

        If, on the other hand, you want to wrap this operator:
        JoinedCurveOffsetPointer operator+(const CurveOffsetPointer& lhs, const CurveOffsetPointer& rhs)
        you need to write 
        Class<ICurveOffset, CurveOffsetPointer> cls(...);
        cls.Operator(This.Storage.Const + This.Storage.Const);
        This will indicate that you want to let the operator work on the storage type (CurveOffsetPointer) and not the 
        reflected type (ICurveOffset).
        */
        template<int decoration>
        struct This {
            template<typename T>
            BinaryOperatorGenerator<This,T,Tags::Subscript> operator[](const T&) const {return BinaryOperatorGenerator<This,T,Tags::Subscript>();}
            template<typename T>
            BinaryOperatorGenerator<This,T,Tags::Assign> operator=(const T&) const {return BinaryOperatorGenerator<This,T,Tags::Assign>();}
            //Allows the expression This.Const
            This<ThisConstType> Const;
            //Allows the expression *This
            This<ThisPointerType> operator*() const {return This<ThisPointerType>();}
            //Allows the expression This.Storage
            This<ThisStorageType> Storage;
        };

        template<>
        struct This<ThisConstStorageType>
        {
        };

        template<>
        struct This<ThisStorageType>
        {
            //Allows the expression This.Storage.Const
            This<ThisConstStorageType> Const;
        };

        template<>
        struct This<ThisConstType> {
            template<typename T>
            BinaryOperatorGenerator<This,T,Tags::Subscript> operator[](const T&) const {return BinaryOperatorGenerator<This,T,Tags::Subscript>();}
            //Allows the expression *This.Const
            This<ThisConstPointerType> operator*() const {return This<ThisConstPointerType>();}
            //Allows the expression This.Const.Storage
            This<ThisConstStorageType> Storage;
        };

        /*
        If you want to support operation between two different types, such as int and double, you can use Other:
        Class<int> cls(...);
        cls.Operator(This.Const + Other<double>());
        Alternatively, if the other type is default constructible, you can write:
        cls.Operator(This.Const + double());
        */
        template<typename T>
        struct Other {};

        /*
        This template allows 'This' and Other to be reinterpreted before generating the final operator function object.
        The default version uses the Input (e.g. the double() in This.Const + double() is handled by this template.)
        */
        template<typename Input,typename T, typename StorageT>
        struct RebindArgument
        {
            using Type = std::conditional_t<Types::IsSmallPodType<Input>::value, Input, const Input&>;
        };
        //'This' will translate to T& (where T is the reflected type in Class<T>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisType>, T, StorageT>
        {
            typedef T& Type;
        };
        //'This.Const' will translate to const T& (where T is the reflected type in Class<T>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisConstType>, T, StorageT>
        {
            using Type = std::conditional_t<Types::IsSmallPodType<T>::value, T, const T&>;
        };
        //'*This' will translate to T* (where T is the reflected type in Class<T>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisPointerType>, T, StorageT>
        {
            typedef T* Type;
        };

        //'*This.Const' will translate to const T* (where T is the reflected type in Class<T>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisConstPointerType>, T, StorageT>
        {
            typedef const T* Type;
        };

        //'This.Storage' will translate to StorageT (where StorageT is std::shared_ptr<T> in Class<T, std::shared_ptr<T>>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisStorageType>, T, StorageT>
        {
            typedef StorageT Type;
        };

        //'This.Storage.Const' will translate to const StorageT& (where StorageT is std::shared_ptr<T> in Class<T, std::shared_ptr<T>>.)
        template<typename T, typename StorageT>
        struct RebindArgument<This<ThisConstStorageType>, T, StorageT>
        {
            typedef const StorageT& Type;
        };

        //If Input == Other<Output>, then we use Output as our Type.
        template<typename Output, typename T, typename StorageT>
        struct RebindArgument<Other<Output>, T, StorageT>
        {
            typedef Output Type;
        };
    }
    //Make 'This' and 'Other' available for use in Class definition.
    const ThisOperators::This<ThisOperators::ThisType> This;
    using ThisOperators::Other;

    namespace ThisOperators {
        ///Used when This is the second argument, then we need to use the BinaryOperatorRName.
        template<typename Lhs,typename Rhs,typename Tag>
        struct BinaryOperatorGenerator {
            template<typename Class, typename StorageClass, typename Callback = void*>
            struct Rebind {
                using NewLhs = typename RebindArgument<Lhs, Class, StorageClass>::Type;
                using NewRhs = typename RebindArgument<Rhs, Class, StorageClass>::Type;
                using Type = BinaryOperator<NewLhs, NewRhs, Tag, Callback>;
            };
            typedef Members::BinaryOperatorRName<Tag> TypeName;
        };
        ///Specialization to support the case where This is the first argument.
        template<int Decoration,typename Rhs,typename Tag>
        struct BinaryOperatorGenerator<This<Decoration>,Rhs,Tag> {
            template<typename Class, typename StorageClass, typename Callback = void*>
            struct Rebind {
                using NewLhs = typename RebindArgument<This<Decoration>, Class, StorageClass>::Type;
                using NewRhs = typename RebindArgument<Rhs, Class, StorageClass>::Type;
                using Type = BinaryOperator<NewLhs,NewRhs,Tag, Callback>;
            };
            typedef Members::BinaryOperatorName<Tag> TypeName;
        };

        template<typename Subject,typename Tag>
        struct UnaryOperatorGenerator {
            template<typename Class, typename StorageClass, typename Callback = void*>
            struct Rebind {
                using NewSubject = typename RebindArgument<Subject, Class, StorageClass>::Type;
                using Type = UnaryOperator<NewSubject,Tag, Callback>;
            };
            typedef Members::UnaryOperatorName<Tag> TypeName;
        };
    }    
}}}}