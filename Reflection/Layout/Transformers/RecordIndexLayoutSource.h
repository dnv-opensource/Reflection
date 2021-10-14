#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Members/IMember.h"
#include "LayoutSource.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/TypeLibraries/IType.h"
#include "Reflection/Attributes/RecordAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class NestedPropertyHandler;
    class NestedProperty;
    //This class provides layouts for a grid, defined by a setter and getter function.
    //It reads a RecordAttribute from the function and an a row and column index. It then searches the record attribute for possible aliases for this cell and alternatively returns the get and set functions + additional arguments specifying the row and column
    class RecordIndexLayoutSource : public LayoutSource 
    {
    public:
        RecordIndexLayoutSource(const TypeLibraries::TypeLibraryPointer& typeLibrary, NestedPropertyHandler& handler, const Members::MemberPointer& function, Variants::Variant row, Variants::Variant column, const std::string& rowName, const std::string& columnName);
        virtual std::string GetName() const override;
        virtual const Attributes::AttributeCollection& GetAttributeCollection() const override;
        virtual Types::DecoratedTypeInfo GetTypeInfo() const override;
        virtual Members::MemberPointer GetGetter() const override;
        virtual Members::MemberPointer GetSetter() const override;
        virtual std::map<std::string, Variants::Variant, TypeUtilities::CompareCaseLess> GetAdditionalArguments() const override;
    private:
        std::vector<LayoutContext> nestedContext;
        Variants::Variant GetNamedArgument(const std::string& name) const;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        TypeLibraries::TypePointer m_type;
        Members::MemberPointer m_setterFunction;
        Members::MemberPointer m_getterfunction;
        Variants::Variant m_row, m_column;
        std::string m_name;
        const Attributes::RecordAttribute& m_recordAttribute;
        const Attributes::PropertyAlias* m_alias;
        NestedProperty* m_nestedProperty;
        NestedPropertyHandler& m_handler;
    };
}}}}