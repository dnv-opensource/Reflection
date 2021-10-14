//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ControlNodeRecordAttributeHandler.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "TypeUtilities/CompareCaseLess.h"
#include "Reflection/Controls/ControlNodeManager.h"
#include "Reflection/Controls/Aspects/RecordAccessorAspect.h"
#include "Reflection/Controls/Aspects/IndexedPropertyAspect.h"
#include "Reflection/Controls/Aspects/TypeAspect.h"
#include "Reflection/Controls/Aspects/IsAliasAspect.h"
#include "Reflection/Controls/Aspects/AliasAspect.h"
#include "boost/algorithm/string/split.hpp"
#include "ControlNodeHandlerUtilities.h"




namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    class RecordNodeAdder {
    public:
        RecordNodeAdder(ControlNode& parent, const Attributes::RecordAttribute& attribute)
            : m_parent(parent)
            , m_attribute(attribute)
            , m_accessor(parent.TryGetAspect<RecordAccessorAspect>())
        {
            if (!parent.GetTypeLibrary())
                return;
            for (const auto& range : attribute.GetRanges())
            {
                m_headers.push_back(range->GetHeadersInRange(Variants::Variant()));
                m_indices.push_back({});
                for (const Variants::Variant& variant : range->GetIndicesInRange(Variants::Variant()))
                    m_indices.back().push_back(Objects::Object(m_parent.GetTypeLibrary(), variant));
                m_indexNames.push_back(range->GetName());
            }
        }
        void AddChildNodes()
        {
            std::map<std::string, Objects::Object, TypeUtilities::CompareCaseLess> namedIndices;
            AddNodesRecursive(namedIndices);

            for (const auto& nestedAlias : m_nestedAliases)
                TryAddConstructor(*nestedAlias.first, nestedAlias.second);
        }
    private:
        bool IsMatchingAlias(const Attributes::PropertyAlias& alias, const std::map<std::string, Objects::Object, TypeUtilities::CompareCaseLess>& namedIndices) const
        {
            if (alias.GetIndices().size() > m_indexNames.size())
                return false;
            for (size_t index = 0; index < alias.GetIndices().size(); ++index)
            {
                auto it = namedIndices.find(m_indexNames.at(index));
                if (it != namedIndices.end())
                {
                    try {
                        if (!(it->second == Objects::Object(m_parent.GetTypeLibrary(), alias.GetIndices().at(index))).As<bool>())
                            return false;
                    }
                    catch (...)
                    {
                        return false;
                    }
                }
            }
            return true;
        }
        const Attributes::PropertyAlias* TryGetAlias(const std::map<std::string, Objects::Object, TypeUtilities::CompareCaseLess>& namedIndices) const
        {
            for (const auto& alias : m_attribute.GetAliases())
            {
                if (IsMatchingAlias(alias, namedIndices))
                    return &alias;
            }
            return nullptr;
        }
        void AddNodesRecursive(std::map<std::string, Objects::Object, TypeUtilities::CompareCaseLess>& namedIndices, size_t index = 0, std::string name = "")
        {
            if (index < m_indices.size())
            {
                if (!name.empty())
                    name += ",";
                for (size_t i = 0; i < m_indices[index].size(); ++i)
                {
                    namedIndices[m_indexNames[index]] = m_indices[index][i];
                    AddNodesRecursive(namedIndices, index + 1, name + m_headers[index][i]);
                }
            }
            else
            {
                std::shared_ptr<ControlNode> child = std::make_shared<ControlNode>(&m_parent);
                std::shared_ptr<ControlNode> aliasNode;
                if (const Attributes::PropertyAlias* alias = TryGetAlias(namedIndices))
                {
                    aliasNode = GetOrCreateAliasedNode(*alias);
                    m_parent.GetManager()->OnInitializeType(*child, aliasNode->GetDecoratedTypeInfo());
                    child->AddAspect<AliasAspect>(aliasNode);
                    child->SetName(alias->GetPropertyName());
                }
                else if (m_attribute.GetRecordTypeCallback())
                {
                    std::vector<Objects::Object> args;
                    for (const std::string& arg : m_attribute.GetRecordTypeCallback()->GetStringArguments())
                    {
                        auto it = namedIndices.find(arg);
                        if (it != namedIndices.end())
                            args.push_back(it->second);
                    }
                    m_parent.GetManager()->OnInitializeType(*child, m_attribute.GetRecordTypeCallback()->InvokeFunction(args));
                }
                else if (m_accessor && m_accessor->GetGetFunction())
                    m_parent.GetManager()->OnInitializeType(*child, m_accessor->GetGetFunction()->GetReturnType());
                child->AddAspect<IndexedPropertyAspect>(m_accessor->GetSetFunction(), m_accessor->GetGetFunction(), namedIndices);
                if(!aliasNode)
                    child->SetName(name);
                child->SetCaption("");
                m_parent.AddChild(child);
            }
        }
        std::shared_ptr<ControlNode> GetOrCreateAliasedNode(const Attributes::PropertyAlias& alias)
        {
            std::vector<std::string> nested;
            boost::algorithm::split(nested, alias.GetPropertyName(), [](char c) {return c == '.'; });
            ControlNode* current = m_parent.GetParent();
            std::shared_ptr<ControlNode> aliasedNode;
            for (size_t i = 0; i < nested.size(); ++i)
            {
                if (!current)
                    return nullptr;
                const std::string& name = nested[i];
                if (current->HasChild(name))
                    aliasedNode = current->GetChild(name);
                else
                {
                    if (i == 0)
                        return nullptr;
                    auto member = current->GetAspect<TypeAspect>().GetType()->Lookup(name, Variants::Variant());
                    if (member)
                    {
                        auto child = std::make_shared<ControlNode>(current, member);
                        child->SetName(name);
                        current->AddChild(child);
                        m_nestedAliases[current].emplace(name, child);
                        aliasedNode = child;
                    }
                }
                if (aliasedNode)
                    current = aliasedNode.get();
                if (current && i == 0)
                    current->AddAspect<IsAliasAspect>();
            }
            return aliasedNode;
        }

        ControlNode& m_parent;
        const Attributes::RecordAttribute& m_attribute;
        const RecordAccessorAspect* m_accessor;
        std::vector<std::vector<std::string>> m_headers;
        std::vector<std::vector<Objects::Object>> m_indices;
        std::vector<std::string> m_indexNames;
        std::map<ControlNode*, std::map<std::string, std::shared_ptr<ControlNode>, TypeUtilities::CompareCaseLess>> m_nestedAliases;
    };

    bool ControlNodeRecordAttributeHandler::OnInitializeMember(ControlNode& node, const Members::MemberPointer& member)
    {
        if (!member || !node.GetTypeLibrary() || !node.GetManager() || !member->GetAttributeCollection().HasAttribute<Attributes::RecordAttribute>())
            return false;
        node.SetName(member->GetName());
        if (!node.HasCaption())
            node.SetCaption("");
        const auto& recordAttribute = member->GetAttributeCollection().GetAttribute<Attributes::RecordAttribute>();
        auto type = node.GetTypeLibrary()->LookupType(member->GetArgumentInfo(0)->GetTypeInfo().GetTypeInfo());
        if (!type)
            return false;
        auto getter = type->Lookup(recordAttribute.GetGetterFunction(), Variants::Variant());
        if (!getter)
            return false;
        node.AddAspect<RecordAccessorAspect>(member, getter);
        RecordNodeAdder(node, recordAttribute).AddChildNodes();
        return true;
    }

}}}}

