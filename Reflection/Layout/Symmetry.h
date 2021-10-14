#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlElement.h"
#include "ILayoutElement.h"
#include "ClonableLayoutPointer.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class Symmetry : public TypedLayoutElement<Symmetry, ILayoutElement>
    {
    public:
        Symmetry(const Symmetry& other) = default;
        Symmetry(Symmetry&&) = default;
        Symmetry& operator=(const Symmetry& other) = default;
        Symmetry& operator=(Symmetry&&) = default;

        template<typename ElementT>
        explicit Symmetry(const std::string& masterPropertyName, const std::string& symmetryCaption, const std::string& documentation,ElementT&& element)
            : m_masterPropertyName(masterPropertyName)
            , m_symmetryCaption(symmetryCaption)
            , m_documentation(documentation)
            , m_element(std::forward<ElementT>(element))
        {
        }
        const std::string& GetDocumentation() const { return m_documentation; }
        const std::string& GetMasterPropertyName() const { return m_masterPropertyName; }
        const std::string& GetSymmetryCaption() const { return m_symmetryCaption; }
        const LayoutElementPointer& GetChild() const { return m_element; }
    private:
        std::string m_symmetryCaption;
        std::string m_masterPropertyName;
        std::string m_documentation;
        ClonableLayoutPointer<ILayoutElement> m_element;
    };
}}}}
