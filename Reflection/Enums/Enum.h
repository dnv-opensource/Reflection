#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Properties/PropertyStaticGet.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Reflection/Members/GlobalType.h"
#include "Reflection/TypeConversions/TrivialConversion.h"
#include "Reflection/TypeConversions/LValueConversion.h"
#include "Reflection/Functions/DefineFunction.h"
#include "Reflection/Operators/DefineOperator.h"
#include "Reflection/Attributes/EnumerableAttribute.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/ParserAttribute.h"
#include "Reflection/Reflect.h"
#include "Reflection/Enums/EnumHelper.h"
#include "Reflection/Members/Arg.h"

#include "Formatting/InsertSpaceBeforeCapitalLetters.h"
#include "Formatting/TrimStart.h"
#include "Formatting/UnionFormatter.h"
#include "Formatting/SurroundWithParenthesis.h"
#include "Formatting/FormattingService.h"
#include "Formatting/ToString.h"
#include "EnumParser.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {

    using DNVS::MoFa::Formatting::InsertSpaceBeforeCapitalLetters;
    using DNVS::MoFa::Formatting::TrimStart;
    using DNVS::MoFa::Formatting::SurroundWithParenthesis;
    using Reflection::Reflect;
    using Members::Arg;
    using namespace Reflection::Attributes;
    using Types::TypeId;
    /// Reflects a C++ enum.
    /// This is a very limited class. We don't support adding any functions, but
    /// we do define all relevant comparison operators and ToString
    /// The class will also have the EnumerableAttribute to allow extracting values to be placed in for example a combo box.
    template<typename EnumT>
    class Enum {
    public:
        Enum(TypeLibraries::TypeLibraryPointer typeLibrary, const std::string& name)
            : m_typeLibrary(typeLibrary)
            , m_alreadyDefined(typeLibrary->LookupType(typeid(EnumT)) != nullptr)
        {
            m_type = typeLibrary->CreateType(typeid(EnumT), name);
            TypeConversions::AddTrivialConversions<EnumT>(typeLibrary->GetConversionGraph());
            TypeConversions::AddLValueConversion<EnumT>(typeLibrary->GetConversionGraph());

            std::weak_ptr<TypeLibraries::IType> weakType = m_type;
            std::weak_ptr<TypeLibraries::ITypeLibrary> weakTypeLibrary = typeLibrary;
            Functions::DefineFunction<EnumT>(typeLibrary, m_type).Function("ToString", [](EnumT value, const DNVS::MoFa::Formatting::FormattingService& formattingService)
            {
                return ToString(value, formattingService);
            }).AddSignature(Arg("formattingService") = DNVS::MoFa::Formatting::FormattingService());
            AddAttribute<Attributes::EnumerableAttribute>(DisableSorting, [weakType]()
            {
                return EnumHelper(weakType.lock()).GetAllReflectedEnumValues();
            });
            AddAttribute<ParserAttribute>(EnumParser(m_type));
            using Operators::This;
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const == This.Const);
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const != This.Const);
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const < This.Const);
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const > This.Const);
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const <= This.Const);
            Operators::DefineOperator<EnumT>(m_type).Operator(This.Const >= This.Const);
        }
        template<typename AttributeT, typename... Args>
        void AddAttribute(Args&&... args)
        {
            GetType()->GetAttributeCollection().AddAttribute(AttributeT(std::forward<Args>(args)...));
        }
        TypeLibraries::TypePointer GetType() const { return m_type; }
        //EnumT& AddDefaultFormatter()
        Properties::PropertyStaticGet<EnumT>& EnumValue(const std::string& name, EnumT value)
        {
            typedef Properties::PropertyStaticGet<EnumT> EnumValueType;
            boost::intrusive_ptr<EnumValueType> enumValue(new EnumValueType(name, m_type->GetConversionGraph(), value));
            EnumValueType& result = *boost::static_pointer_cast<EnumValueType>(m_type->AddMember(name, enumValue));
            if (m_defaultFormatter)
                enumValue->SetFormatter(*m_defaultFormatter);
            m_typeLibrary->LookupType(typeid(Members::GlobalType))->AddMember(name, enumValue);
            return result;
        }
        Properties::PropertyStaticGet<EnumT>& InternalEnumValue(const std::string& name, EnumT value)
        {
            typedef Properties::PropertyStaticGet<EnumT> EnumValueType;
            auto member = EnumHelper(m_type).GetMember((int)value);
            if (member)
                return *boost::static_pointer_cast<EnumValueType>(member);

            boost::intrusive_ptr<EnumValueType> enumValue(new EnumValueType(name, m_type->GetConversionGraph(), value));
            m_type->AddMember("__Internal" + boost::lexical_cast<std::string>(m_type->GetUnsortedMembers().size()) + "__", enumValue);
            if (m_defaultFormatter)
                enumValue->SetFormatter(*m_defaultFormatter);
            return *enumValue;
        }
        void AddFormatter(const DNVS::MoFa::Formatting::IStringFormatter& formatter)
        {
            m_defaultFormatter.reset(formatter.Clone());
            EnumHelper(m_type).SetFormatter(formatter);
        }
        bool AlreadyDefined() const { return m_alreadyDefined; }
    private:
        std::unique_ptr<DNVS::MoFa::Formatting::IStringFormatter> m_defaultFormatter;
        TypeLibraries::TypePointer m_type;
        bool m_alreadyDefined;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
    };
}}}}
