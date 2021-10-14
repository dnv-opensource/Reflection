#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
namespace DNVS {namespace MoFa {namespace Services { namespace Allocators {
    struct GenericDeleter {
        template<typename T>
        void operator()(T* ptr) const { delete ptr; }
    };
}}}}