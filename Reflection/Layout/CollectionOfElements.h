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

    class CollectionOfElements : public TypedLayoutElement<CollectionOfElements, ILayoutElement>
    {
    public:
        CollectionOfElements(CollectionOfElements&& other) = default;
        CollectionOfElements(const CollectionOfElements& other) = default;
        CollectionOfElements& operator=(CollectionOfElements&& other) = default;
        CollectionOfElements& operator=(const CollectionOfElements& other) = default;

        template<typename... ElementsT>
        explicit CollectionOfElements(ElementsT&&... args)
        {
            PushBack(std::forward<ElementsT>(args)...);
        }
        //Terminating call
        void PushBack(){}
        template<typename ElementT, typename... ElementsT>
        void PushBack(ElementT&& firstElement, ElementsT&&... args)
        {
            m_elements.push_back(std::forward<ElementT>(firstElement));
            PushBack(std::forward<ElementsT>(args)...);
        }
        //Terminating call
        void PushFront() {}
        template<typename ElementT, typename... ElementsT>
        void PushFront(ElementT&& firstElement, ElementsT&&... args)
        {
            m_elements.push_front(std::forward<ElementT>(firstElement));
            PushFront(std::forward<ElementsT>(args)...);
        }
        void Clear()
        {
            m_elements.clear();
        }
        using ElementList = std::list<ClonableLayoutPointer<ILayoutElement>>;
        using const_iterator = ElementList::const_iterator;
        using iterator = ElementList::iterator;

        iterator begin() { return m_elements.begin(); }
        iterator end() { return m_elements.end(); }
        const_iterator begin() const { return m_elements.begin(); }
        const_iterator end() const { return m_elements.end(); }
        size_t size() const { return m_elements.size(); }
        void erase(const iterator& it) { m_elements.erase(it); }
        void SetName(const std::string& name) { m_name = name; }
        const std::string& GetName() const { return m_name; }
        virtual void SetParentType(TypeLibraries::TypeLibraryPointer typeLibrary, Types::DecoratedTypeInfo& typeInfo)
        {
            for(auto& element : m_elements)
                element->SetParentType(typeLibrary, typeInfo);
        }
    protected:
        std::string m_name;
        std::list<ClonableLayoutPointer<ILayoutElement>> m_elements;
    };
}}}}
