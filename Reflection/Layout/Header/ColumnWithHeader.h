#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IColumn.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Layout {
#pragma warning(push)
#pragma warning(disable:4275)
    class REFLECTION_IMPORT_EXPORT ColumnWithHeader : public IColumn 
    {
    public:
        ColumnWithHeader(const std::string& header);        
        ColumnWithHeader(const ColumnWithHeader& other) = default;
        virtual std::string GetHeader(TypeLibraries::TypeLibraryPointer typeLibrary, const Types::DecoratedTypeInfo& parentType, bool withPostfix = false) const override;
        virtual std::string GetDocumentation() const override;
        virtual Variants::Variant GetDefault() const override;
        void SetDocumentation(const std::string& documentation);
        void SetDefault(const Variants::Variant& value);
    private:
        std::string m_header;
        std::string m_documentation;
        Variants::Variant m_default;
    };
#pragma warning(pop)
}}}}
