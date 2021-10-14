#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IContent.h"
namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class ContentObjectAtIndex {
    public:
        ContentObjectAtIndex(const std::shared_ptr<IContent>& content)
            : m_content(content)
        {}
        Objects::Object operator()(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Members::MemberPointer& member, size_t index) const;
    private:
        std::shared_ptr<IContent> m_content;
    };
}}}}