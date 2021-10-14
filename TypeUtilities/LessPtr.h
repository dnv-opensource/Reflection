#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <type_traits>

namespace DNVS { namespace MoFa { namespace TypeUtilities {

    template<typename SmartPointerT>
    class LessPtr
    {
    public:
        using is_transparent = int;
        using ValueType = std::remove_pointer_t<decltype(std::declval<SmartPointerT>().get())>;
        
        bool operator()(const SmartPointerT& lhs, const SmartPointerT& rhs) const
        {
            return lhs.get() < rhs.get();
        }
        bool operator()(const SmartPointerT& lhs, const ValueType* rhs) const { return lhs.get() < rhs; }
        bool operator()(const ValueType* lhs, const SmartPointerT& rhs) const { return lhs < rhs.get(); }
        bool operator()(const ValueType* lhs, const ValueType* rhs) const { return lhs < rhs; }
    };
}}}