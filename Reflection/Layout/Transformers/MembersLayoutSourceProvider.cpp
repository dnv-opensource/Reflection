//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MembersLayoutSourceProvider.h"
#include "PropertyLayoutSource.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/TypeLibraries/IType.h"
#include "TypeLayoutSource.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    MemberLayoutSourceProvider::MemberLayoutSourceProvider(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Types::DecoratedTypeInfo& info)
        : m_typeLibrary(typeLibrary)
        , m_info(info)
    {
    }

    std::shared_ptr<ILayoutSource> MemberLayoutSourceProvider::LookupLayoutSource(const std::string& name)
    {
        auto type = m_typeLibrary->LookupType(m_info.GetTypeInfo());
        if (type)
        {
            auto member = type->Lookup(name, Variants::Variant());
            if (member)
                return std::make_shared<PropertyLayoutSource>(member);
            if (name == "__this__")
                return std::make_shared<TypeLayoutSource>(name, m_info, type->GetAttributeCollection());
        }
        return nullptr;
    }

}}}}


