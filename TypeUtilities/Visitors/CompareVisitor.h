#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IVisitable.h"
#include <typeinfo>

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    class CompareVisitor : virtual public IVisitor
    {
    public:
        CompareVisitor(const IVisitable& lhs, const IVisitable& rhs) : m_lhs(lhs) , m_rhs(rhs) {}
        operator bool()
        {
            
            if (typeid(m_lhs) != typeid(m_rhs))
                return false;
            m_lhs.Accept(*this);
            return m_result;
        }
    protected:
        const IVisitable& m_lhs;
        const IVisitable& m_rhs;
        bool m_result;
    };

    template<typename... Args>
    class CompareVisitors;

    template<typename T, typename... Args>
    class CompareVisitors<T, Args...> : public TypedVisitor<T>, public CompareVisitors<Args...>
    {
    public:
        typedef CompareVisitors BaseClass;
        CompareVisitors(const IVisitable& lhs, const IVisitable& rhs) : CompareVisitors<Args...>(lhs,rhs) {}
        virtual void Visit(const T& lhs) override
        {
            m_result = Compare(lhs, *dynamic_cast<const T*>(&m_rhs));
        }
        virtual bool Compare(const T& lhs, const T& rhs) = 0;
    };

    template<>
    class CompareVisitors<> : public CompareVisitor
    {
    public:
        typedef CompareVisitors BaseClass; 
        CompareVisitors(const IVisitable& lhs, const IVisitable& rhs) : CompareVisitor(lhs, rhs) {}

    };
}}}