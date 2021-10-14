#pragma once
#include <functional>
#include <assert.h>

//
//  intrusive_ptr.hpp
//
//  Copyright (c) 2001, 2002 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/intrusive_ptr.html for documentation.
//


namespace DNVS {namespace MoFa {namespace TypeUtilities {

    //
    //  intrusive_ptr
    //
    //  A smart pointer that uses intrusive reference counting.
    //
    //  Relies on unqualified calls to
    //  
    //      void intrusive_ptr_add_ref(T * p);
    //      void intrusive_ptr_release(T * p);
    //
    //          (p != 0)
    //
    //  The object is responsible for destroying itself.
    //

    template<class T> class IntrusivePointer
    {
    private:

        typedef IntrusivePointer this_type;

    public:

        typedef T element_type;

        IntrusivePointer(): px( 0 )
        {
        }

        IntrusivePointer( T * p, bool add_ref = true ): px( p )
        {
            if( px != 0 && add_ref ) intrusive_ptr_add_ref( px );
        }

        template<class U>
        IntrusivePointer( IntrusivePointer<U> const & rhs )
            : px( rhs.get() )
        {
            if( px != 0 ) intrusive_ptr_add_ref( px );
        }


        IntrusivePointer(IntrusivePointer const & rhs): px( rhs.px )
        {
            if( px != 0 ) intrusive_ptr_add_ref( px );
        }

        ~IntrusivePointer()
        {
            if( px != 0 ) intrusive_ptr_release( px );
        }

        template<class U> IntrusivePointer & operator=(IntrusivePointer<U> const & rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        IntrusivePointer(IntrusivePointer&& rhs): px( rhs.px )
        {
            rhs.px = 0;
        }

        IntrusivePointer& operator=(IntrusivePointer&& rhs)
        {
            this_type( static_cast<IntrusivePointer&& >( rhs ) ).swap(*this);
            return *this;
        }

        IntrusivePointer& operator=(IntrusivePointer const & rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        IntrusivePointer& operator=(T * rhs)
        {
            this_type(rhs).swap(*this);
            return *this;
        }

        void reset()
        {
            this_type().swap( *this );
        }

        void reset( T * rhs )
        {
            this_type( rhs ).swap( *this );
        }

        T * get() const
        {
            return px;
        }

        T & operator*() const
        {
            assert( px != 0 );
            return *px;
        }

        T * operator->() const
        {
            assert( px != 0 );
            return px;
        }

        // implicit conversion to "bool"
        typedef T * this_type::*unspecified_bool_type;
        operator unspecified_bool_type() const // never throws
        {
            return px == 0? 0: &this_type::px;
        }
        // operator! is redundant, but some compilers need it
        bool operator! () const // never throws
        {
            return px == 0;
        }

        void swap(IntrusivePointer & rhs)
        {
            T * tmp = px;
            px = rhs.px;
            rhs.px = tmp;
        }

    private:

        T * px;
    };

    template<class T, class U> inline bool operator==(IntrusivePointer<T> const & a, IntrusivePointer<U> const & b)
    {
        return a.get() == b.get();
    }

    template<class T, class U> inline bool operator!=(IntrusivePointer<T> const & a, IntrusivePointer<U> const & b)
    {
        return a.get() != b.get();
    }

    template<class T, class U> inline bool operator==(IntrusivePointer<T> const & a, U * b)
    {
        return a.get() == b;
    }

    template<class T, class U> inline bool operator!=(IntrusivePointer<T> const & a, U * b)
    {
        return a.get() != b;
    }

    template<class T, class U> inline bool operator==(T * a, IntrusivePointer<U> const & b)
    {
        return a == b.get();
    }

    template<class T, class U> inline bool operator!=(T * a, IntrusivePointer<U> const & b)
    {
        return a != b.get();
    }

    template<class T> inline bool operator<(IntrusivePointer<T> const & a, IntrusivePointer<T> const & b)
    {
        return std::less<T *>()(a.get(), b.get());
    }

    template<class T> void swap(IntrusivePointer<T> & lhs, IntrusivePointer<T> & rhs)
    {
        lhs.swap(rhs);
    }

    // mem_fn support

    template<class T> T * get_pointer(IntrusivePointer<T> const & p)
    {
        return p.get();
    }

    template<class T, class U> IntrusivePointer<T> static_pointer_cast(IntrusivePointer<U> const & p)
    {
        return static_cast<T *>(p.get());
    }

    template<class T, class U> IntrusivePointer<T> const_pointer_cast(IntrusivePointer<U> const & p)
    {
        return const_cast<T *>(p.get());
    }

    template<class T, class U> IntrusivePointer<T> dynamic_pointer_cast(IntrusivePointer<U> const & p)
    {
        return dynamic_cast<T *>(p.get());
    }

}}}