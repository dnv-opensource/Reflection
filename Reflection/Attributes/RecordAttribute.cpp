//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "RecordAttribute.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {

    IndexRange::IndexRange(size_t lowerIndex, size_t upperIndex, const std::string& indexName) : m_lowerIndex(lowerIndex)
        , m_upperIndex(upperIndex)
        , m_indexName(indexName)
    {
        if (m_upperIndex < m_lowerIndex)
            throw std::runtime_error("Upper index (" + std::to_string(m_upperIndex) + ") of " + indexName + " must be greater or equal to lower index (" + std::to_string(m_lowerIndex) + ")");
    }

    std::string IndexRange::GetName() const
    {
        return m_indexName;
    }

    std::vector<Variants::Variant> IndexRange::GetIndicesInRange(const Variants::Variant& object) const
    {
        std::vector<Variants::Variant> indicesInRange;
        indicesInRange.reserve(m_upperIndex - m_lowerIndex + 1);
        for (size_t i = m_lowerIndex; i <= m_upperIndex; ++i)
            indicesInRange.push_back(Variants::VariantService::ReflectType(i));
        return indicesInRange;
    }

    std::vector<std::string> IndexRange::GetHeadersInRange(const Variants::Variant& object) const
    {
        if (!m_headerNames.empty())
            return m_headerNames;
        else
        {
            std::vector<std::string> names;
            for (size_t i = m_lowerIndex; i <= m_upperIndex; ++i)
                names.push_back(std::to_string(i));
            return names;
        }
    }

    void IndexRange::InitializeHeaderNames()
    {

    }

    const std::string& PropertyAlias::GetPropertyName() const
    {
        return m_propertyName;
    }

    const std::vector<Variants::Variant>& PropertyAlias::GetIndices() const
    {
        return m_indices;
    }

    bool PropertyAlias::IsAliasOf(const std::string& name) const
    {
        if (_strcmpi(m_propertyName.c_str(), name.c_str()) == 0)
            return true;
        size_t index = m_propertyName.find_first_of('.');
        if (index != name.length())
            return false;
        for (size_t i = 0; i < index; ++i)
            if (tolower(name[i]) != tolower(m_propertyName[i]))
                return false;
        return true;
    }

    void PropertyAlias::InitializeIndices()
    {

    }

}}}}

