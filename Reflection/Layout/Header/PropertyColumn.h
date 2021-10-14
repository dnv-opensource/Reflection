#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GenericColumn.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
    class REFLECTION_IMPORT_EXPORT PropertyColumn : public GenericColumn<PropertyColumn>
    {
    public:
        PropertyColumn(const std::string& propertyName, const std::string& header = "");
        virtual Objects::Object Query(const Objects::Object& data) const override;
        virtual Types::DecoratedTypeInfo GetType(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType) const override;
    private:
        std::string m_propertyName;
    };
}}}}
