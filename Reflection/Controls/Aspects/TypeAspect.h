#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IControlAspect.h"
#include "Reflection/Members/IMemberFwd.h"
namespace DNVS {namespace MoFa {namespace Reflection {namespace Controls {
    //Returns the attribute collection representing a basic type.
    //This is always the lowest priority aspect. Any aspects found earlier in the list of aspects will be checked before this.
    class REFLECTION_IMPORT_EXPORT TypeAspect : public IControlAspectT<TypeAspect>
    {
    public:
        TypeAspect(const TypeLibraries::TypePointer& type);
        ~TypeAspect();
        const Attributes::AttributeCollection& GetAttributeCollection() const override;
        bool operator==(const TypeAspect& other) const override;
        std::string ToString() const override;
        const TypeLibraries::TypePointer& GetType() const { return m_type; }
    private:
        TypeLibraries::TypePointer m_type;
    };
}}}}