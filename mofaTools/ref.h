#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#pragma warning(disable:4786)
#include <map>
#include "TypeUtilities/IntrusivePointer.h"
#include <stddef.h>

namespace mofa {
    template <class T>
    class ref {
    private:
        DNVS::MoFa::TypeUtilities::IntrusivePointer<T> m_ptr;
    public:
        const DNVS::MoFa::TypeUtilities::IntrusivePointer<T>& GetPtr() const { return m_ptr; }
        ref(T* object = 0) : m_ptr(object) {}
        ref(const ref<T>& rhs) : m_ptr(rhs.m_ptr) {}
        ref(ref<T>&& rhs) : m_ptr(std::move(rhs)) {}
        template<typename U>
        ref(const ref<U>& rhs) : m_ptr(rhs.GetPtr()) {}
        ref& operator=(const ref<T>& rhs) { m_ptr = rhs.m_ptr; return *this; }
        ~ref() {}

        T* operator->() const { return m_ptr.get(); }
        operator T*() const { return m_ptr.get(); }
        T* get() const { return m_ptr.get(); }
        int count() const { return get()?intrusive_ptr_count(get()):0; }

        bool operator<(const ref<T>& rhs) const { return get() < rhs.get(); }
        bool operator==(T* rhs) { return get() == rhs; }
        bool operator!=(T* rhs) { return get() != rhs; }
        bool operator!() { return !get(); }

        void swap(ref<T>& other) {
            std::swap(m_ptr, other.m_ptr);
        }

    };

    template<typename T>
    T* get_pointer(const ref<T>& ptr) {
        return ptr.get();
    }

    template<class T>
    struct less_ref : public std::binary_function< ref<T>, ref<T>, bool> {
        bool operator()(const ref<T>& x, const ref<T>& y) const {
            return (*x.get()) < (*y.get());
        }
    };
}

