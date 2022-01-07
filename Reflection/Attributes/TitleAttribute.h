#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"
#include <string>

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    //Defines the title of a control
    class TitleAttribute : public IAttribute
    {
    public:
        TitleAttribute(const std::string& title) : m_title(title) {}
        const std::string& GetTitle() const { return m_title; }
    private:
        std::string m_title;
    };

}}}}
