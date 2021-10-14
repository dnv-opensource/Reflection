#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/Types/ITypeFormatter.h"
#include "ITypeLibrary.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT TypeLibraryNamedTypeFormatter : public Types::ITypeFormatter {
    public:
        TypeLibraryNamedTypeFormatter(ITypeLibrary* typeLibrary,std::shared_ptr<Types::ITypeFormatter> fallback);
        virtual std::string FormatType( const Types::DecoratedTypeInfo& typeInfo ) const;
    private:
        std::shared_ptr<Types::ITypeFormatter> m_fallback;
        ITypeLibrary* m_typeLibrary;
    };
#pragma warning(pop)
}}}}