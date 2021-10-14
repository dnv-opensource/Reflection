#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"
#include "Reflection/Variants/InternalVariantService.h"

#include <vector>
#include <type_traits>
#include <memory>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members { namespace ArgumentForwarder {


    template<size_t Arity>
    class NoConvertArgumentListVararg 
    {
    public:
        NoConvertArgumentListVararg(const std::vector<Variants::Variant>& args) 
            :   m_arguments(args)
        {
        }
        template<typename T, size_t Index>
        T Get() 
        {
            return GetImpl<T>(std::integral_constant<size_t, Index>());
        }        
        template<typename T, size_t Index>
        T GetImpl(std::integral_constant<size_t, Index>) 
        {
            return Variants::InternalVariantService::UnreflectUnchecked<T>(m_arguments[Index]);
        }
        template<typename T>
        T GetImpl(std::integral_constant<size_t, Arity>) 
        {
            typedef typename std::remove_const<typename std::remove_reference<T>::type>::type ContainerType;
            typedef typename ContainerType::value_type ValueType;
            ContainerHolder<ContainerType>* holder = new ContainerHolder<ContainerType>;
            m_holder.reset(holder);
            for(size_t i = Arity; i<m_arguments.size(); ++i) 
            {
                holder->container.insert(holder->container.end(), Variants::InternalVariantService::UnreflectUnchecked<ValueType>(m_arguments[i]));
            }
            return holder->container;
        }
    private:
        class ContainerHolderImpl 
        {
        public:
            virtual ~ContainerHolderImpl() {}
        };
        template<typename ContainerType>
        class ContainerHolder : public ContainerHolderImpl
        {
        public:
            virtual ~ContainerHolder() {}
            ContainerType container;
        };
        std::unique_ptr<ContainerHolderImpl> m_holder;
        const std::vector<Variants::Variant>& m_arguments;
    };
}}}}}
