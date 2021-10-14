//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionLayoutSourceProvider.h"
#include "boost\algorithm\string\predicate.hpp"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "PropertyLayoutSource.h"
#include "TypeLayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    using namespace Attributes;
    FunctionLayoutSourceProvider::FunctionLayoutSourceProvider(const TypeLibraries::TypeLibraryPointer& typeLibrary, Members::MemberPointer function)
        : m_typeLibrary(typeLibrary)
        , m_function(function)
    {
    }


    std::shared_ptr<ILayoutSource> FunctionLayoutSourceProvider::LookupLayoutSource(const std::string& name)
    {
        if (!m_function)
            return nullptr;
        for (size_t i = 0; i < m_function->GetArity(); ++i)
        {
            if (boost::iequals(m_function->GetArgumentInfo(i)->GetName(), name))
            {
                auto type = m_typeLibrary->LookupType(m_function->GetReturnType().GetTypeInfo());
                if (type)
                {
                    auto member = type->Lookup(name, Variants::Variant());
                    if (member)
                        return std::make_shared<PropertyLayoutSource>(member);
                    else
                        return std::make_shared<TypeLayoutSource>(name, m_function->GetArgumentInfo(i)->GetTypeInfo(), type->GetAttributeCollection());
                }
                else
                    return std::make_shared<TypeLayoutSource>(name, m_function->GetArgumentInfo(i)->GetTypeInfo(), AttributeCollection::GetEmptyCollection());
            }
        }
        if (name == "__this__")
        {
            auto type = m_typeLibrary->LookupType(m_function->GetReturnType().GetTypeInfo());
            if (type)
                return std::make_shared<TypeLayoutSource>(name, m_function->GetReturnType(), type->GetAttributeCollection());
        }
        return nullptr;
    }

}}}}
