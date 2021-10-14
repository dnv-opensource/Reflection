#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <stdexcept>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    //Use this class instead of std::unique_ptr if you want to guarantee that it is never null. This is to save you from checking null pointers all the time.
    template<typename T>
    class NonNullablePtr {
    public:
        NonNullablePtr(T* ptr) : m_ptr(nullptr)
        {
            reset(ptr);
        }
        NonNullablePtr(std::unique_ptr<T>&& ptr) : NonNullablePtr(ptr.release())
        {}
        ~NonNullablePtr()
        {
            std::default_delete<T>()(m_ptr);
        }
        NonNullablePtr(NonNullablePtr&&) = default;
        NonNullablePtr(const NonNullablePtr&) = delete;
        NonNullablePtr& operator=(NonNullablePtr&) = delete;
        NonNullablePtr& operator=(NonNullablePtr&&) = default;
        T* operator->() const { return m_ptr; }
        T* get() const { return m_ptr; }
        void reset(T* ptr)
        {
            if(m_ptr)
                std::default_delete<T>()(m_ptr);
            m_ptr = ptr;
            if (!m_ptr)
                throw std::runtime_error("Pointer cannot be null");
        }
    private:
        T* m_ptr;
    };
    template<typename T>
    T* get_pointer(const NonNullablePtr<T>& ptr)
    {
        return ptr.get();
    }
}}}