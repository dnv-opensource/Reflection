#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IntrusivePointer.h"
#include <atomic>

namespace DNVS { namespace MoFa { namespace TypeUtilities { 

    template<typename Derived>
    class IntrusiveClass {
    public:
        IntrusiveClass() : m_count(0) {}
        IntrusiveClass(const IntrusiveClass& other) : m_count(0) {}
        friend void intrusive_ptr_add_ref(IntrusiveClass<Derived>* p) {
            ++p->m_count;
        }
        friend void intrusive_ptr_release(IntrusiveClass<Derived>* p) {
            if(--p->m_count==0) {
                delete static_cast<Derived*>(p);
            }
        }
        friend size_t intrusive_ptr_count(IntrusiveClass<Derived>* p) {
            return p->m_count;
        }
    private:        
        std::atomic<size_t> m_count;
    };    

}}}