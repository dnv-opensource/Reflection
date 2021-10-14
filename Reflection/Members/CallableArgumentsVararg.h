#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/Variant.h"

#include "Reflection/TypeConversions/TypeConversionService.h"

#include "IMember.h"

#include <vector>
#include <type_traits>
#include "boost\mpl\size_t.hpp"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {

    
    template<size_t Arity>
    class CallableArgumentsVararg 
    {
    public:
        CallableArgumentsVararg(const std::vector<Variants::Variant>& args, MemberPointer member) 
            :   m_arguments(args)
            ,   m_member(member) 
        {
            for(size_t i = m_arguments.size(); i<Arity; ++i)
            {
                m_arguments.push_back(m_member->GetArgumentInfo(i)->GetDefault());
            }
        }
        template<typename T, size_t Index>
        Types::ReturnType<T> Get() 
        {
            return GetImpl<T>(boost::mpl::size_t<Index>());
        }        
        template<typename T, size_t Index>
        Types::ReturnType<T> GetImpl(boost::mpl::size_t<Index>) 
        {
            TypeConversions::TypeConversionService service(m_member->GetConversionGraph());
            return service.IntrusiveAs<T>(m_arguments[Index]);
        }
        template<typename T>
        T GetImpl(boost::mpl::size_t<Arity>) 
        {
            typedef typename std::remove_const<typename std::remove_reference<T>::type>::type ContainerType;
            typedef typename ContainerType::value_type ValueType;
            ContainerHolder<ContainerType>* holder = new ContainerHolder<ContainerType>;
            TypeConversions::TypeConversionService service(m_member->GetConversionGraph());

            m_holder.reset(holder);
            for(size_t i = Arity; i<m_arguments.size(); ++i) 
            {
                holder->container.insert(holder->container.end(), service.IntrusiveAs<ValueType>(m_arguments[i]));
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
        std::auto_ptr<ContainerHolderImpl> m_holder;
        std::vector<Variants::Variant> m_arguments;
        MemberPointer m_member;
    };
}}}}
