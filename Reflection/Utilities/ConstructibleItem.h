#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Formatting/FormattingService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    /*
    This item will take any type, and checks that the constructors of this class have a defined signature (named arguments)
    Also check that these named arguments match read/write properties defined on the class.
    */
    class REFLECTION_IMPORT_EXPORT ConstructibleItem
    {
    public:
        ConstructibleItem(TypeLibraries::TypePointer type, TypeLibraries::TypeLibraryPointer typeLibrary);
        TypeLibraries::TypePointer GetType() const;
        //This method throws an exception if the type is not well defined.
        void Validate() const;
    private:
        TypeLibraries::TypePointer m_type;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        void ValidateConstructorHasNoUnnamedArguments(Members::MemberPointer constructor) const;
        void ValidateConstructorHasNoMismatchedNamedArguments(Members::MemberPointer constructor) const;
    };

    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructibleItem**);
    std::string REFLECTION_IMPORT_EXPORT ToString(const ConstructibleItem& item, const DNVS::MoFa::Formatting::FormattingService& formattingService);

}}}}
