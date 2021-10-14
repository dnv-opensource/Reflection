#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {
    
    class ArgumentTypeVisitor {
    public:
        ArgumentTypeVisitor(size_t index) : m_index(index){}
        template<typename T,size_t Index>
        void Visit() {
            if(Index==m_index) m_typeInfo=Types::TypeId<T>();
        }
        Types::DecoratedTypeInfo GetTypeInfo() const {return m_typeInfo;}
    private:
        Types::DecoratedTypeInfo m_typeInfo;
        size_t m_index;
    };

}}}}