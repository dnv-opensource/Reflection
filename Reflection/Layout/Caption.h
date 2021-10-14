#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypedLayoutElement.h"
#include "ILayoutElement.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Caption : public TypedLayoutElement<Caption, ILayoutElement>
    {
    public:
        Caption(const Caption&) = default;
        Caption(Caption&&) = default;
        Caption& operator=(const Caption&) = default;
        Caption& operator=(Caption&&) = default;

        explicit Caption(const std::string& caption) : m_caption(caption) {}
        const std::string& GetCaption() const { return m_caption; }
    private:
        std::string m_caption;
    };
}}}}
