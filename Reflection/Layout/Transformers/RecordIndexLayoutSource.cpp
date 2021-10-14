//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RecordIndexLayoutSource.h"
#include "Reflection/Attributes/RecordAttribute.h"
#include "boost/algorithm/string/split.hpp"
#include "NestedPropertyHandler.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    RecordIndexLayoutSource::RecordIndexLayoutSource(const TypeLibraries::TypeLibraryPointer& typeLibrary, NestedPropertyHandler& handler, const Members::MemberPointer& function, Variants::Variant row, Variants::Variant column, const std::string& rowName, const std::string& columnName)
        : m_typeLibrary(typeLibrary)
        , m_type(typeLibrary->LookupType(function->GetArgumentInfo(0)->GetTypeInfo().GetTypeInfo()))
        , m_setterFunction(function)
        , m_row(row)
        , m_column(column)
        , m_recordAttribute(function->GetAttributeCollection().GetAttribute<Attributes::RecordAttribute>())
        , m_alias(nullptr)
        , m_handler(handler)
        , m_nestedProperty(nullptr)
    {
        m_name = rowName + "," + columnName;
        if (!m_type)
            throw std::runtime_error("Undefined type");
        m_getterfunction = m_type->Lookup(m_recordAttribute.GetGetterFunction());
        if(!m_recordAttribute.GetAliases().empty() && m_recordAttribute.GetRanges().size() ==2)
        {
            Objects::Object first(m_typeLibrary, GetNamedArgument(m_recordAttribute.GetRanges().front()->GetName()));
            Objects::Object second(m_typeLibrary, GetNamedArgument(m_recordAttribute.GetRanges().back()->GetName()));
            for(const Attributes::PropertyAlias& alias : m_recordAttribute.GetAliases())
            {
                if(alias.GetIndices().size() == 2)
                {
                    if((Objects::Object(m_typeLibrary, alias.GetIndices()[0]) == first).As<bool>() &&
                       (Objects::Object(m_typeLibrary, alias.GetIndices()[1]) == second).As<bool>())
                    {
                        m_alias = &alias;
                        break;
                    }
                }
            }
        }
        if(m_alias)
            m_nestedProperty = m_handler.CreateProperty(m_alias->GetPropertyName());
    }

    std::string RecordIndexLayoutSource::GetName() const
    {
        if (m_alias)
            return m_alias->GetPropertyName();
        return m_name;
    }

    const Attributes::AttributeCollection& RecordIndexLayoutSource::GetAttributeCollection() const
    {
        if (m_nestedProperty)
            return m_nestedProperty->GetContext().GetPropertyAttributeCollection();
        return m_setterFunction->GetAttributeCollection();
    }

    Types::DecoratedTypeInfo RecordIndexLayoutSource::GetTypeInfo() const
    {
        if (m_nestedProperty)
            return m_nestedProperty->GetContext().type;
        if (m_recordAttribute.GetRecordTypeCallback())
        {
            std::vector<Objects::Object> args;
            for (const std::string& name : m_recordAttribute.GetRecordTypeCallback()->GetStringArguments())
                args.push_back(Objects::Object(m_typeLibrary, GetNamedArgument(name)));

            return m_recordAttribute.GetRecordTypeCallback()->InvokeFunction(args);
        }
        return m_getterfunction->GetReturnType();
    }

    Members::MemberPointer RecordIndexLayoutSource::GetGetter() const
    {
        if(m_nestedProperty)
            return m_nestedProperty->GetContext().getter;
        return m_getterfunction;
    }

    Members::MemberPointer RecordIndexLayoutSource::GetSetter() const
    {
        if (m_nestedProperty)
            return m_nestedProperty->GetContext().setter;
        return m_setterFunction;
    }

    std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> RecordIndexLayoutSource::GetAdditionalArguments() const
    {
        std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> additionalArguments;
        if(!m_nestedProperty)
        {
            for(const auto& range : m_recordAttribute.GetRanges())
                additionalArguments.emplace(range->GetName(), GetNamedArgument(range->GetName()));
        }
        return additionalArguments;
    }

    Variants::Variant RecordIndexLayoutSource::GetNamedArgument(const std::string& name) const
    {
        if (m_recordAttribute.GetRanges().size() != 2)
            return Variants::Variant();
        if (_strcmpi(m_recordAttribute.GetRanges().front()->GetName().c_str(), name.c_str()) == 0)
            return m_row;
        if (_strcmpi(m_recordAttribute.GetRanges().back()->GetName().c_str(), name.c_str()) == 0)
            return m_column;
        return Variants::Variant();
    }


}}}}

