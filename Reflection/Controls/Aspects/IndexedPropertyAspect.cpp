//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IndexedPropertyAspect.h"
#include "Reflection/Members/IMember.h"
#include "../../Contents/IReflectionContentsService.h"
#include "Services/ServiceProvider.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {

    IndexedPropertyAspect::IndexedPropertyAspect(const Members::MemberPointer& setFunction, const Members::MemberPointer& getFunction, const IndexMap& indices)
        : m_indices(indices)
        , m_setFunction(setFunction)
        , m_getFunction(getFunction)
    {

    }

    IndexedPropertyAspect::~IndexedPropertyAspect()
    {

    }

    const Attributes::AttributeCollection& IndexedPropertyAspect::GetAttributeCollection() const
    {
        if (m_setFunction && !m_setFunction->GetAttributeCollection().IsEmpty())
            return m_setFunction->GetAttributeCollection();
        else if(m_getFunction && !m_getFunction->GetAttributeCollection().IsEmpty())
            return m_getFunction->GetAttributeCollection();
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string IndexedPropertyAspect::ToString() const
    {
        std::string result = "IndexedPropertyAspect({";
        bool isFirst = true;
        if (m_setFunction)
            result += m_setFunction->GetName();
        else
            result += "nullptr";
        result += ", ";
        if (m_getFunction)
            result += m_getFunction->GetName();
        else
            result += "nullptr";
        result += ", ";
        for (const auto& indexPair : m_indices)
        {
            if (!isFirst)
                result += ", ";
            else
                isFirst = false;
            result += "{\"" + indexPair.first + "\", " + Objects::ToString(indexPair.second, Formatting::FormattingService()) + "}";
        }
        result + "})";
        return result;
    }

    const Members::MemberPointer& IndexedPropertyAspect::GetGetFunction() const
    {
        return m_getFunction;
    }

    const Members::MemberPointer& IndexedPropertyAspect::GetSetFunction() const
    {
        return m_setFunction;
    }

    Contents::Content IndexedPropertyAspect::TryGetPropertyContent(const Contents::Content& parent, bool trySimplify)
    {
        auto scriptingService = Services::ServiceProvider::Instance().TryGetService<Contents::IReflectionContentsService>();
        if (scriptingService)
        {
            if (!scriptingService->IsLookupExpression(parent.GetScriptText()+";") && !parent.HasStoredObject())
                return Contents::Content();
            if(!parent.GetObject().IsValid())
                return Contents::Content();
        }
        if (Members::MemberPointer getter = GetGetFunction())
        {
            std::vector<Contents::Content> args;
            for (size_t i = 1; i < getter->GetArity(); ++i)
            {
                auto it = m_indices.find(getter->GetArgumentInfo(i)->GetName());
                if (it == m_indices.end())
                    return Contents::Content();
                args.push_back(Contents::Content(it->second));
            }
            return parent.TryInvoke(getter, args, trySimplify);
        }
        return Contents::Content();
    }

    Contents::Content IndexedPropertyAspect::TrySetPropertyContent(const Contents::Content& parent, const Contents::Content& value)
    {
        if (Members::MemberPointer setter = GetSetFunction())
        {
            std::vector<Contents::Content> args;
            for (size_t i = 1; i < setter->GetArity()-1; ++i)
            {
                auto it = m_indices.find(setter->GetArgumentInfo(i)->GetName());
                if (it == m_indices.end())
                    return Contents::Content();
                args.push_back(Contents::Content(it->second));
            }
            args.push_back(value);
            return parent.TryInvoke(setter, args);
        }
        return Contents::Content();
    }

    bool IndexedPropertyAspect::SupportAssign() const
    {
        return m_setFunction != nullptr;
    }

    const IndexedPropertyAspect::IndexMap& IndexedPropertyAspect::GetIndices() const
    {
        return m_indices;
    }

    bool IndexedPropertyAspect::operator==(const IndexedPropertyAspect& other) const
    {
        if (m_getFunction != other.m_getFunction)
            return false;
        if (m_setFunction != other.m_setFunction)
            return false;
        return m_indices == other.m_indices;
    }

}}}}
