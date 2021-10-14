//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SymmetryAspect.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {


    SymmetryAspect::SymmetryAspect(const std::string& masterPropertyName, const std::string& documentation)
        : m_masterPropertyName(masterPropertyName)
        , m_documentation(documentation)
    {}

    SymmetryAspect::~SymmetryAspect()
    {}

    const Attributes::AttributeCollection& SymmetryAspect::GetAttributeCollection() const
    {
        return Attributes::AttributeCollection::GetEmptyCollection();
    }

    std::string SymmetryAspect::ToString() const
    {
        return "SymmetryAspect(\"" + m_masterPropertyName + "\" , \"" + m_documentation + "\")";
    }

    const std::string& SymmetryAspect::GetMasterPropertyName() const
    {
        return m_masterPropertyName;
    }

    const std::string& SymmetryAspect::GetDocumentation() const
    {
        return m_documentation;
    }

    bool SymmetryAspect::operator==(const SymmetryAspect& other) const
    {
        return other.m_masterPropertyName == m_masterPropertyName && m_documentation == other.m_documentation;
    }

}}}}

