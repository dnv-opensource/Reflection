#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutElement.h"
#include <list>
#include "TypedLayoutElement.h"
#include "ClonableLayoutPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class ElementWithOneChild : public TypedLayoutElement<ElementWithOneChild, ILayoutElement>
    {
    public:
        ElementWithOneChild() = default;
        ElementWithOneChild(ElementWithOneChild&&) = default;
        ElementWithOneChild(const ElementWithOneChild& other) = default;
        ElementWithOneChild& operator=(ElementWithOneChild&&) = default;
        ElementWithOneChild& operator=(const ElementWithOneChild& other) = default;

        template<typename T>
        explicit ElementWithOneChild(T&& element)
        {
            m_child = std::forward<T>(element);
        }
        const LayoutElementPointer& GetChild() const { return m_child; }
        void SetChild(const LayoutElementPointer& child) { m_child = child; }
        virtual void SetParentType(TypeLibraries::TypeLibraryPointer typeLibrary, Types::DecoratedTypeInfo& typeInfo)
        {
            if(m_child)
                m_child->SetParentType(typeLibrary, typeInfo);
        }
    private:
        ClonableLayoutPointer<ILayoutElement> m_child;
    };
}}}}
