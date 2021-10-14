#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename T, typename Getter, typename Setter>
    class ScopedCallback
    {
    public:
        ScopedCallback(ScopedCallback&& other)
            : m_getter(other.m_getter)
            , m_setter(other.m_setter)
            , m_valid(true)
            , m_backup(other.m_backup)
        {
            arg.m_valid = false;
        }
        ScopedCallback(Getter getter, Setter setter, T newVal)
            : ScopedCallback(getter, setter)
        {
            m_setter(newVal);
        }
        ScopedCallback(Getter getter, Setter setter)
            : m_getter(getter)
            , m_setter(setter)
            , m_valid(true)
        {
            m_backup = m_getter();
        }
        ~ScopedCallback()
        {
            if(m_valid)
                m_setter(m_backup);
        }
        ScopedCallback& operator=(const T& arg)
        {
            m_setter(arg);
            return *this;
        }
    private:
        bool m_valid;
        Getter m_getter;
        Setter m_setter;
        T m_backup;
    };

    template<typename T>
    class ScopedVariable
    {
    public:
        ScopedVariable(T&& arg)
            : m_valueToScope(arg.m_valueToScope) 
            , m_backup(arg.m_backup)
        {
            arg.m_backup = arg.m_valueToScope;
        }
        ScopedVariable(T& valueToScope, const T& newVal)
            : ScopedVariable(valueToScope)
        {
            m_valueToScope = newVal;
        }
        ScopedVariable(T& valueToScope)
            : m_valueToScope(valueToScope)
            , m_backup(valueToScope)
        {}
        ~ScopedVariable()
        {
            m_valueToScope = m_backup;
        }
        ScopedVariable& operator=(const T& arg)
        {
            m_valueToScope = arg;
            return *this;
        }
    private:
        T& m_valueToScope;
        T m_backup;
    };
    template<typename T>
    ScopedVariable<T> Scope(T& valueToScope, const T& newVal)
    {
        return ScopedVariable<T>(valueToScope, newVal);
    }

    template<typename T>
    ScopedVariable<T> Scope(T& valueToScope)
    {
        return ScopedVariable<T>(valueToScope);
    }

    template<typename T, typename Getter, typename Setter>
    ScopedCallback<T, Getter, Setter> Scope(Getter getter, Setter setter)
    {
        return ScopedCallback<T, Getter, Setter>(getter, setter);
    }

    template<typename T, typename Getter, typename Setter>
    ScopedCallback<T, Getter, Setter> Scope(Getter getter, Setter setter, const T& newVal)
    {
        return ScopedCallback<T, Getter, Setter>(getter, setter, newVal);
    }

}}}