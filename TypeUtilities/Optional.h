#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS { namespace MoFa { namespace TypeUtilities { 
    template<typename T>
    class Optional {
    public:
        Optional(const T& value) : m_value(value), m_initialized(true) {}
        Optional() : m_initialized(false) {}
        Optional& operator =(const T& value) { m_initialized = true; m_value = value; return *this; }
        Optional& operator =(Optional& other) { m_initialized = other.m_initialized; m_value = other.m_value; return *this; }

		typedef T Optional::*unspecified_bool_type;
		operator unspecified_bool_type() const // never throws
		{
			return m_initialized ? &Optional::m_value : nullptr;
		}
		bool operator!() const { return !m_initialized; }
        T& operator*() { return m_value; }
        const T& operator*() const { return m_value; }
    private:
        T m_value;
        bool m_initialized;
    };

	template<typename T>
	bool operator<(const Optional<T>& lhs, const Optional<T>& rhs)
	{
		if (!rhs)
			return false;
		if (!lhs)
			return true;
		return *lhs < *rhs;
	}
}}}