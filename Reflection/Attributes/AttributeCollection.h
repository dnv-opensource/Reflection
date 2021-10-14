#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <map>
#include <memory>
#include <typeindex>
#include "AttributeWrapper.h"
#include "Reflection/Config.h"
#include <vector>

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    class REFLECTION_IMPORT_EXPORT AttributeCollection
    {
    public:
        template<typename T>
        bool HasAttribute() const
        {
            return GetAttributeImpl(typeid(T)) != nullptr;
        }
        template<typename T>
        const T& GetAttribute() const
        {
            std::type_index typeIndex(typeid(T));
            IAttributeWrapper* attribute = GetAttributeImpl(typeIndex);
            if (attribute)
            {
                if(AttributeWrapper<T>* wrapper = dynamic_cast<AttributeWrapper<T>*>(attribute))
                    return wrapper->Get();
                if (DynamicAttributeWrapper* wrapper = dynamic_cast<DynamicAttributeWrapper*>(attribute))
                {
                    const T* attrib = dynamic_cast<const T*>(wrapper->Get().get());
                    if (attrib)
                        return *attrib;
                }
            }
            throw std::runtime_error("Attribute " + std::string(typeIndex.name()) + " not found");
        }
        template<typename T>
        void AddAttribute(T&& attribute)
        {
            m_attributes[typeid(T)] = std::make_shared<AttributeWrapper<T>>(std::move(attribute));
        }
        void AddAttribute(std::shared_ptr<IAttribute> attribute)
        {
            m_attributes[typeid(*attribute)] = std::make_shared<DynamicAttributeWrapper>(attribute);
        }
        void AddBaseAttribute(AttributeCollection& baseAttribute)
        {
            if(&baseAttribute != this)
                m_baseAttributes.push_back(&baseAttribute);
        }
        void RemoveAttribute(std::type_index typeIndex)
        {
            m_attributes.erase(typeIndex);
        }
        static const AttributeCollection& GetEmptyCollection();
        bool IsEmpty() const { return m_attributes.empty(); }
    private:
        IAttributeWrapper* GetAttributeImpl(const std::type_index& typeIndex) const
        {
            auto it = m_attributes.find(typeIndex);
            if (it != m_attributes.end())
                return it->second.get();
            for (AttributeCollection* base : m_baseAttributes)
            {
                IAttributeWrapper* wrapper = base->GetAttributeImpl(typeIndex);
                if (wrapper)
                    return wrapper;
            }
            return nullptr;
        }
        static AttributeCollection s_emptyCollection;
        std::map<std::type_index, std::shared_ptr<IAttributeWrapper>> m_attributes;
        std::vector<AttributeCollection*> m_baseAttributes;
    };
}}}}