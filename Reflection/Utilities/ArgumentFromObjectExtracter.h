#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMemberFwd.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Variants/Variant.h"
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Attributes {
    class GenerateConstructorArgumentsAttribute;
}}}}

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    /**
    Sometimes you want to be able to extract derived values from your class:
    Example:
    For Vector3d(2,5,6), if you ask for its 'y' value, you want it to return 5.
    We do this by looking up the property value 'y' in the type and evaluating this property on the input object.
    For a more complex example:    
    ZPlane3d(5) will create a plane3d. This plane will not have a z value, and only certain planes can be constructed with this constructor.
    If we want to extract this z, we need to use the GenerateConstructorArgumentsAttribute assigned to this particular constructor.
    It will evaluate the plane and assign z if possible.
    This class encapsulates this behavior.
    */
    class REFLECTION_IMPORT_EXPORT ArgumentFromObjectExtracter
    {
    public:
        ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, Members::MemberPointer constructor, size_t argumentIndex);
        ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, Members::MemberPointer constructor, Members::MemberPointer getter, Members::MemberPointer setter);
        ArgumentFromObjectExtracter(TypeLibraries::TypeLibraryPointer typeLibrary, const std::type_index& typeIndex, const std::string& argumentName, Members::MemberPointer constructor);
        ~ArgumentFromObjectExtracter();
        Variants::Variant operator()(const Variants::Variant& object) const;
        Members::MemberPointer GetGetter() const;
        Members::MemberPointer GetSetter() const;
        std::string GetName() const;
        std::string GetCaption() const;
        bool HasSetter() const;
        Types::DecoratedTypeInfo GetType() const;
    private:
        bool TryPopulatePropertyGetSet(const std::type_index& typeIndex, const std::string& name);
        bool TryPopulateFromGenerateConstructorArgumentsAttribute(Members::MemberPointer constructor);

        Members::MemberPointer m_constructor;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        size_t m_argumentIndex;
        Members::MemberPointer m_getter;
        Members::MemberPointer m_setter;
        const Attributes::GenerateConstructorArgumentsAttribute* m_generateConstructorArgumentsAttribute;
        void DetectConstructorArgumentIndexFromArgumentName(const std::string& argumentName);
    };
}}}}