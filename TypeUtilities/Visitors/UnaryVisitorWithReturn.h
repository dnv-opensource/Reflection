#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IVisitable.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename ReturnT>
    class UnaryVisitorWithReturn : virtual public IVisitor
    {
    public:
        UnaryVisitorWithReturn(const IVisitable& subject) : m_subject(subject) {}
        operator ReturnT()
        {
            m_subject.Accept(*this);
            return m_result;
        }
    protected:
        const IVisitable& m_subject;
        ReturnT m_result;
    };

    template<typename ReturnT, typename... Args>
    class UnaryVisitorsWithReturn;

    template<typename ReturnT, typename T, typename... Args>
    class UnaryVisitorsWithReturn<ReturnT, T, Args...>  : public TypedVisitor<T>, public UnaryVisitorsWithReturn<ReturnT, Args...>
    {
    public:
        typedef UnaryVisitorsWithReturn BaseType;
        UnaryVisitorsWithReturn(const IVisitable& subject) : UnaryVisitorsWithReturn<ReturnT, Args...>(subject) {}
        virtual void Visit(const T& subject) override
        {
            m_result = VisitAndReturn(subject);
        }
        virtual ReturnT VisitAndReturn(const T& subject) = 0;
    };

    template<typename ReturnT>
    class UnaryVisitorsWithReturn<ReturnT> : public UnaryVisitorWithReturn<ReturnT>
    {
    public:
        typedef UnaryVisitorsWithReturn BaseType;
        UnaryVisitorsWithReturn(const IVisitable& subject) : UnaryVisitorWithReturn<ReturnT>(subject) {}
    };
}}}