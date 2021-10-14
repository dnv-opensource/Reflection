#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "Reflection/Layout/VerticalElements.h"
#include "Reflection/Layout/HorizontalElements.h"
#include "Reflection/Layout/GroupBox.h"
#include "Reflection/Layout/TextBox.h"
#include "Reflection/Layout/ComboBox.h"
#include "Reflection/Layout/CheckBox.h"
#include "Reflection/Layout/RadioButtons.h"
#include "Reflection/Layout/Image.h"
#include "Reflection/Layout/Grid.h"
#include "Reflection/Layout/Table.h"
#include "Reflection/Layout/Symmetry.h"
#include "Reflection/Layout/EmbeddedElement.h"
#include "Reflection/Layout/OverloadSelector.h"
#include "Reflection/Layout/DropdownButton.h"
#include "Reflection/Layout/DynamicArea.h"
#include "Reflection/Layout/Collapsable.h"
#include "Reflection/Layout/Selector.h"
#include "Reflection/Layout/SelectorLookup.h"
#include "Reflection/Layout/TabControl.h"
#include "Reflection/Layout/RadioButton.h"
#include "Reflection/Layout/Caption.h"
#include "Reflection/Layout/Buttons.h"
#include "Reflection/Layout/Slider.h"
#include "IAttribute.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    ///https://wiki.dnvgl.com/display/DNVSP/Reflection+Attributes/#ReflectionAttributes-LayoutAttribute
    using namespace Layout;
    class LayoutAttribute : public IAttribute {
    public:
        template<typename ElementT>
        explicit LayoutAttribute(ElementT&& element)
            : m_layoutElement(MakeElement(std::forward<ElementT>(element)))
        {}
        LayoutAttribute()
        {}
        LayoutElementPointer GetLayout() const { return m_layoutElement; }
    private:
        LayoutElementPointer m_layoutElement;
    };
}}}}
