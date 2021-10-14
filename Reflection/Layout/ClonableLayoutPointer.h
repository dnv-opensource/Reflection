#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include "AutoControl.h"
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    template<typename LayoutT>    
    class ClonableLayoutPointer {
    public:
        template<typename DerivedT>
        ClonableLayoutPointer(DerivedT&& derived, std::enable_if_t<std::is_base_of_v<LayoutT,DerivedT>, void*> = nullptr)
            : m_layout(std::make_shared<DerivedT>(std::forward<DerivedT>(derived)))
        {}
        template<typename DerivedT>
        ClonableLayoutPointer(const DerivedT& derived, std::enable_if_t<std::is_base_of_v<LayoutT, DerivedT>, void*> = nullptr)
            : m_layout(std::make_shared<DerivedT>(derived))
        {}
        template<typename DerivedT>
        ClonableLayoutPointer(std::shared_ptr<DerivedT>&& derived, std::enable_if_t<std::is_base_of_v<LayoutT, DerivedT>, void*> = nullptr)
            : m_layout(std::move(derived))
        {}
        template<typename DerivedT>
        ClonableLayoutPointer(const std::shared_ptr<DerivedT>& derived, std::enable_if_t<std::is_base_of_v<LayoutT, DerivedT>, void*> = nullptr)
            : m_layout(derived)
        {}

        //If you input a text string, it will be automatically converted to AutoControl, but don't enable this overload if AutoControl does not inherit from LayoutT.
        template<typename StringT>
        ClonableLayoutPointer(const StringT& text, std::enable_if_t<CanConvertToAutoControl<StringT, LayoutT>, void*> = nullptr)
            : m_layout(std::make_shared<AutoControl>(text))
        {}

        ClonableLayoutPointer() {}
        ClonableLayoutPointer(const std::shared_ptr<LayoutT>& layout)
            : m_layout(layout)
        {}
        ClonableLayoutPointer(ClonableLayoutPointer&& other) = default;
        ClonableLayoutPointer(const ClonableLayoutPointer& other)
            : m_layout(other.m_layout ? std::dynamic_pointer_cast<LayoutT>(other.m_layout->Clone()) : nullptr)
        {}
        ClonableLayoutPointer& operator=(const std::shared_ptr<LayoutT>& layout)
        {
            m_layout = layout;
            return *this;
        }
        ClonableLayoutPointer& operator=(ClonableLayoutPointer&& other) = default;
        ClonableLayoutPointer& operator=(const ClonableLayoutPointer& other)
        {
            if (this != &other)
                m_layout = other.m_layout ? std::dynamic_pointer_cast<LayoutT>(other.m_layout->Clone()) : nullptr;
            return *this;
        }
        LayoutT* operator->() const { return m_layout.get(); }
        LayoutT* get() const { return m_layout.get(); }
        operator const std::shared_ptr<LayoutT>&() const { return m_layout; }
        explicit operator bool() const { return m_layout != nullptr; }
        const LayoutT& operator*() const { return *m_layout; }        
        const std::shared_ptr<LayoutT>& GetLayoutPointer() const { return m_layout; }
    private:
        std::shared_ptr<LayoutT> m_layout;
    };
}}}}
namespace std {
    template<typename T, typename LayoutT>
    std::shared_ptr<T> dynamic_pointer_cast(const DNVS::MoFa::Reflection::Layout::ClonableLayoutPointer<LayoutT>& layout)
    {
        return std::dynamic_pointer_cast<T>(layout.GetLayoutPointer());
    }
}