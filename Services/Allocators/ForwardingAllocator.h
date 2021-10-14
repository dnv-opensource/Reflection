#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAllocator.h"
#include <memory>

namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {

    template<typename T>
    class ForwardingAllocator 
    {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T value_type;
        typedef ForwardingAllocator<T> alloc_type;
        template <class Other> struct rebind {
            typedef ForwardingAllocator<Other> other;
        };
        ForwardingAllocator()
            :   m_allocator(nullptr)
        {}
        ForwardingAllocator(AllocatorPointer allocator)
            :   m_allocator(allocator)
        {}
        ForwardingAllocator(const ForwardingAllocator& other) 
            :   m_allocator(other.m_allocator)
        {
        }
        template <class Other> 
        ForwardingAllocator(const ForwardingAllocator<Other>& other) 
            :   m_allocator(other)
        {
        }
        ~ForwardingAllocator() {}
        operator AllocatorPointer() const {
            return m_allocator;
        }
        pointer address(reference _X) const 
        { return (&_X); }
        const_pointer address(const_reference _X) const
        { return (&_X); }

        pointer allocate(size_type _n, const void* = 0) 
        {
            if(m_allocator)
                return static_cast<pointer>(m_allocator->Allocate(_n, sizeof(value_type), typeid(value_type)));
            else
                return static_cast<pointer>(::operator new[](sizeof(value_type)*_n));
        }
        void deallocate(pointer _P, size_type __n)
        {
            if(m_allocator)
                m_allocator->Deallocate(_P, __n);
            else
                delete[](_P);
        }
        size_t max_size() const 
        { 
            return (_SIZT)(-1) / sizeof (value_type);
        }

        void construct(pointer _Ptr)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type();
        }
        template<typename A0>
        void construct(pointer _Ptr, A0&& a0)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0));
        }
        template<typename A0, typename A1>
        void construct(pointer _Ptr, A0&& a0, A1&& a1)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1));
        }
        template<typename A0, typename A1, typename A2>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2));
        }
        template<typename A0, typename A1, typename A2, typename A3>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2, A3&& a3)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3));
        }
        template<typename A0, typename A1, typename A2, typename A3, typename A4>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2, A3&& a3, A4&& a4)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), std::forward<A4>(a4));
        }
        template<typename A0, typename A1, typename A2, typename A3, typename A4,typename A5>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), std::forward<A4>(a4), std::forward<A5>(a5));
        }
        template<typename A0, typename A1, typename A2, typename A3, typename A4,typename A5, typename A6>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5, A6&& a6)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), std::forward<A4>(a4), std::forward<A5>(a5), std::forward<A6>(a6));
        }
        template<typename A0, typename A1, typename A2, typename A3, typename A4,typename A5, typename A6, typename A7>
        void construct(pointer _Ptr, A0&& a0, A1&& a1, A2&& a2, A3&& a3, A4&& a4, A5&& a5, A6&& a6, A7&& a7)
        {	// construct object at _Ptr with value _Val
            new ((void*)_Ptr) value_type(std::forward<A0>(a0), std::forward<A1>(a1), std::forward<A2>(a2), std::forward<A3>(a3), std::forward<A4>(a4), std::forward<A5>(a5), std::forward<A6>(a6), std::forward<A7>(a7));
        }


        void destroy(pointer _Ptr)
        {	// destroy object at _Ptr
            _Ptr->~value_type();
        }
        bool operator==(const ForwardingAllocator<T>& other) const {return m_allocator == other.m_allocator;}
        bool operator!=(const ForwardingAllocator<T>& other) const {return m_allocator != other.m_allocator;}
    private:
        AllocatorPointer m_allocator;
    };

    template <> 
    class ForwardingAllocator<void> {
    public:
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef void* pointer;
        typedef const void* const_pointer;
        typedef void value_type;
        typedef ForwardingAllocator<void> alloc_type;

        template <class Other> struct rebind {
            typedef ForwardingAllocator<Other> other;
        };
        ForwardingAllocator(AllocatorPointer allocator)
            :   m_allocator(allocator)
        {}
        operator AllocatorPointer() const {
            return m_allocator;
        }
    private:
        AllocatorPointer m_allocator;
    };
}}}}