#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/Config.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    enum class TabPosition {
        AboveTab,
        InTab,
        BelowTab,
    };

    class TabAttribute : public IAttribute
    {
    public:        
        TabAttribute(TabPosition pos, const std::string& name = "")
            : m_tabPosition(pos)
            , m_name(name)
        {}
        TabAttribute(const std::string& name)
            : m_name(name)
            , m_tabPosition(TabPosition::InTab)
        {}        
        const std::string& GetName() const { return m_name; }
        TabPosition GetTabPosition() const { return m_tabPosition; }
    private:
        std::string m_name;
        TabPosition m_tabPosition;
    };
}}}}