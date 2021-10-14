#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Types/DecoratedTypeInfo.h"
#include <type_traits>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    template<int Arity>
    class ArgumentTypeVisitorVararg {
    public:
        ArgumentTypeVisitorVararg(size_t index) : m_index(index){}
        template<typename T,int Index>
        void Visit() {
            if(Index==m_index) m_typeInfo=VisitImpl<Index>::Apply<T>();
        }
        Types::DecoratedTypeInfo GetTypeInfo() const {return m_typeInfo;}
    private:
        template<int Index>
        struct VisitImpl {
            template<typename T>
            static Types::DecoratedTypeInfo Apply() {
                return Types::TypeId<T>();
            }
        };
        template<>
        struct VisitImpl<Arity> {
            template<typename T>
            static Types::DecoratedTypeInfo Apply() {
                typedef typename std::remove_const<typename std::remove_reference<T>::type>::type ContainerType;
                typedef typename ContainerType::value_type ValueType;
                return Types::TypeId<ValueType>();
            }
        };
        Types::DecoratedTypeInfo m_typeInfo;
        size_t m_index;
    };
}}}}