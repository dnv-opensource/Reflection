#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "InvokerImpl.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename Signature, bool HasAllocatorAsLastArgument = false>
    struct Constructor;   
    
    template<typename ReturnTypeT, typename... Args>
    struct Constructor<ReturnTypeT* (Args...), false>
    {
        typedef ReturnTypeT* ReturnType;
        typedef ReturnType Signature (Args...);

        template<typename ArgumentsT,typename AllocatorT>
        static ReturnType Invoke(ArgumentsT& args,const AllocatorT& allocator) {
            typedef typename AllocatorT::rebind<ReturnTypeT>::other MyAllocator;
            auto callback = [&allocator](Args&&... args) -> ReturnType {
                MyAllocator myAlloc(allocator);
                return new(myAlloc.allocate(1)) ReturnTypeT(std::forward<Args>(args)...);
            };
            return InvokeImpl(args, callback, TypeList<Args...>());
        }
    };

    template<typename ReturnTypeT, typename... Args>
    struct Constructor<ReturnTypeT* (Args...), true>
    {
        typedef ReturnTypeT* ReturnType;
        typedef ReturnType Signature(Args...);

        template<typename ArgumentsT, typename AllocatorT>
        static ReturnType Invoke(ArgumentsT& args, const AllocatorT& allocator) {
            typedef typename AllocatorT::rebind<ReturnTypeT>::other MyAllocator;
            auto callback = [&allocator](Args&&... args) -> ReturnType {
                MyAllocator myAlloc(allocator);
                return new(myAlloc.allocate(1)) ReturnTypeT(std::forward<Args>(args)..., myAlloc);
            };
            return InvokeImpl(args, callback, TypeList<Args...>());
        }
    };
    template<typename ReturnTypeT, typename... Args>
    struct Constructor<ReturnTypeT (Args...), false>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType Signature(Args...);

        template<typename ArgumentsT, typename AllocatorT>
        static ReturnType Invoke(ArgumentsT& args, const AllocatorT& allocator) {
            auto callback = [&allocator](Args&&... args) -> ReturnType {
                return ReturnTypeT(std::forward<Args>(args)...);
            };
            return InvokeImpl(args, callback, TypeList<Args...>());
        }
    };

    template<typename ReturnTypeT, typename... Args>
    struct Constructor<ReturnTypeT (Args...), true>
    {
        typedef ReturnTypeT ReturnType;
        typedef ReturnType Signature(Args...);

        template<typename ArgumentsT, typename AllocatorT>
        static ReturnType Invoke(ArgumentsT& args, const AllocatorT& allocator) {
            typedef typename AllocatorT::rebind<ReturnTypeT>::other MyAllocator;
            auto callback = [&allocator](Args&&... args) -> ReturnType {
                MyAllocator myAlloc(allocator);
                return ReturnTypeT(std::forward<Args>(args)..., myAlloc);
            };
            return InvokeImpl(args, callback, TypeList<Args...>());
        }
    };
}}}
