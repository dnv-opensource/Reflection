#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Layout/OverloadSelector.h"
#include "Reflection/Layout/Selector.h"
#include "Reflection/Layout/TabControl.h"
#include "Reflection/Layout/VerticalElements.h"
#include "Reflection/Layout/DynamicArea.h"
#include "Reflection/Layout/ControlElement.h"
#include <list>
#include <string>


namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Allows selection of multiple functions to be displayed in a dialog.
    */
    class FunctionSelectorAttribute {
    public:        
        template<typename... Args>
        FunctionSelectorAttribute(const Layout::OverloadSelector& layout, const Args&... functionNames)
        : m_overloadSelector(layout)
        {
            CollectFunctionNames(functionNames...);
            m_overloadSelector.SetName("Functions");
        }

        template<typename... Args>
        FunctionSelectorAttribute(const Layout::ControlElement& selectorControl, const Args&... functionNames)
            : FunctionSelectorAttribute(Layout::OverloadSelector("Functions", Layout::VerticalElements(Layout::Selector(std::dynamic_pointer_cast<Layout::ControlElement>(selectorControl.Clone())), Layout::DynamicArea())), functionNames...)
        {}
        template<typename... Args>
        FunctionSelectorAttribute(const Layout::TabControl& tabControl, const Args&... functionNames)
            : FunctionSelectorAttribute(Layout::OverloadSelector("Functions", tabControl), functionNames...)
        {}
        const Layout::OverloadSelector& GetOverloadSelector() const { return m_overloadSelector; }
        const std::list<std::string>& GetMemberNames() const { return m_memberNames; }
    private:
        void CollectFunctionNames()
        {}

        template<typename... Args>
        void CollectFunctionNames(const std::string& name, const Args&... functionNames)
        {
            m_memberNames.push_back(name);
            CollectFunctionNames(functionNames...);
        }
        Layout::OverloadSelector m_overloadSelector;
        std::list<std::string> m_memberNames;
    };
}}}}
