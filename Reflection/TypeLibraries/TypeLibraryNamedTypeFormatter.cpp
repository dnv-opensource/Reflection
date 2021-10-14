//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeLibraries/TypeLibraryNamedTypeFormatter.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    TypeLibraryNamedTypeFormatter::TypeLibraryNamedTypeFormatter( ITypeLibrary* typeLibrary,std::shared_ptr<Types::ITypeFormatter> fallback )
        :   m_typeLibrary(typeLibrary)
        ,   m_fallback(fallback)
    {
    }

    std::string TypeLibraryNamedTypeFormatter::FormatType( const Types::DecoratedTypeInfo& typeInfo ) const
    {
        TypePointer type=m_typeLibrary->LookupType(typeInfo.GetTypeInfo());
        if(type && !type->GetName().empty()) return type->GetName();
        return m_fallback->FormatType(typeInfo);
    }

}}}}