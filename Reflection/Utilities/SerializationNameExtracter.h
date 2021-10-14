#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    //Extracts the serialization name from any member. If it has a SerializationAttribute, this will be used,
    //otherwise the name of the property or the name of the constructor will be used.
    class REFLECTION_IMPORT_EXPORT SerializationNameExtracter
    {
    public:
        SerializationNameExtracter(TypeLibraries::TypeLibraryPointer typeLibrary);
        ~SerializationNameExtracter();
        std::string GetName(const Members::MemberPointer& member) const;
        std::string GetName(const TypeLibraries::TypePointer& type) const;
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
}}}}

