#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "TypedLayoutElement.h"
#include "ILayoutElement.h"
#include "LayoutContext.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Controls/ControlNode.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    class ControlElement : public TypedLayoutElement<ControlElement, ILayoutElement>
    {
    public:
        ControlElement(const ControlElement&) = default;
        ControlElement(ControlElement&&) = default;
        ControlElement& operator=(const ControlElement&) = default;
        ControlElement& operator=(ControlElement&&) = default;

        ControlElement(const std::shared_ptr<Controls::ControlNode>& controlNode)
            : m_name(controlNode->GetName())
            , m_caption(controlNode->GetCaption())
            , m_controlNode(controlNode)
        {}
        explicit ControlElement(const std::string& name, const std::string& caption= "__USENAME__")
            : m_name(name)
            , m_caption(caption)
            , m_attributeCollection(nullptr)
        {}
        std::string GetName() const { return m_name; }
        std::string GetCaption() const { return m_caption; }
        void SetName(const std::string& name ) { m_name = name; }
        void SetCaption(const std::string& caption) { m_caption = caption; }
        bool HasCaption() const { return m_caption != "__USENAME__"; }
		void SetTypeInfo(const Types::DecoratedTypeInfo& typeInfo) { m_typeInfo = typeInfo; }
		const Types::DecoratedTypeInfo& GetTypeInfo() const { return m_typeInfo; }
        LayoutContext& GetContext() { return m_context; }
        const LayoutContext& GetContext() const { return m_context; }
        void SetAttributeCollection(const Attributes::AttributeCollection& collection) { m_attributeCollection = &collection; }
        const Attributes::AttributeCollection& GetAttributeCollection() const 
        {
            if(m_attributeCollection)
                return *m_attributeCollection;
            else 
                return Attributes::AttributeCollection::GetEmptyCollection();
        }
        const std::shared_ptr<Controls::ControlNode>& GetControlNode() const { return m_controlNode; }
        void SetControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            m_controlNode = controlNode;
            if (m_controlNode)
            {
                m_name = m_controlNode->GetName();
                if (m_controlNode->HasCaption())
                    m_caption = m_controlNode->GetCaption();
                else
                    m_caption = m_controlNode->GetName();
            }
        }
        std::shared_ptr<Controls::ControlNode> TryGetMatchingControlNode(const std::shared_ptr<Controls::ControlNode>& controlNode)
        {
            if (!controlNode)
                return m_controlNode;
            if (controlNode == m_controlNode)
                return controlNode;
            if (m_name.empty())
                return controlNode;
            if (_strcmpi(controlNode->GetName().c_str(), m_name.c_str()) == 0)
                return controlNode;
            if (m_caption == "__USENAME__" && m_name == controlNode->GetCaption())
                return controlNode;
            return controlNode->LookupRelatedNode(m_name);
        }
		std::shared_ptr<Attributes::AttributeCollection> GetAttribtes() const {
			return m_attributes;
		}
		std::shared_ptr<Attributes::AttributeCollection> GetOrCreateAttributes()
		{
			if (!m_attributes)
				m_attributes = std::make_shared<Attributes::AttributeCollection>();
			return m_attributes;
		}
    private:
		std::shared_ptr<Attributes::AttributeCollection> m_attributes;
        std::shared_ptr<Controls::ControlNode> m_controlNode;
        std::string m_name;
        std::string m_caption;
		Types::DecoratedTypeInfo m_typeInfo;
        LayoutContext m_context;
        const Attributes::AttributeCollection* m_attributeCollection;
    };
}}}}
