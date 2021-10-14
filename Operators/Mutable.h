#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Operators\Tags.h>

namespace DNVS { namespace MoFa { namespace Operators {
    template<typename Tag>
    struct Mutable
    {
        static const bool value = false;
    };

    template<>
    struct Mutable<Tags::Multiplies>
    {
        static const bool value = true;
        typedef Tags::Divides opposite_tag;
    };

    template<>
    struct Mutable<Tags::Divides>
    {
        static const bool value = false;
        typedef Tags::Multiplies opposite_tag;
    };

    template<>
    struct Mutable<Tags::Plus>
    {
        static const bool value = true;
        typedef Tags::Minus opposite_tag;
    };

    template<>
    struct Mutable<Tags::Minus>
    {
        static const bool value = false;
        typedef Tags::Plus opposite_tag;
    };

    template<>
    struct Mutable<Tags::Less>
    {
        static const bool value = false;
        typedef Tags::Greater opposite_tag;
    };

    template<>
    struct Mutable<Tags::Greater>
    {
        static const bool value = false;
        typedef Tags::Less opposite_tag;
    };

    template<>
    struct Mutable<Tags::LessEqual>
    {
        static const bool value = false;
        typedef Tags::GreaterEqual opposite_tag;
    };

    template<>
    struct Mutable<Tags::GreaterEqual>
    {
        static const bool value = false;
        typedef Tags::LessEqual opposite_tag;
    };

    template<>
    struct Mutable<Tags::EqualTo>
    {
        static const bool value = true;
    };

    template<>
    struct Mutable<Tags::NotEqualTo>
    {
        static const bool value = true;
    };

}}}