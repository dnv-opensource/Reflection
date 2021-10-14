//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ConstructibleItem.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/ValidationAttribute.h"
#include "Reflection/Attributes/DeprecatedAttribute.h"
#include "Reflection/Members/MemberFormatter.h"
#include "sstream"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    

    ConstructibleItem::ConstructibleItem(TypeLibraries::TypePointer type, TypeLibraries::TypeLibraryPointer typeLibrary)
        : m_type(type)
        , m_typeLibrary(typeLibrary)
    {

    }

    TypeLibraries::TypePointer ConstructibleItem::GetType() const
    {
        return m_type;
    }

    void ConstructibleItem::Validate() const
    {
        auto constructor = m_type->Lookup(Members::ConstructorString);
        if (!constructor)
            return;
        for (auto constructor : constructor->GetOverloads())
        {
            if (constructor->GetAttributeCollection().HasAttribute<Attributes::DeprecatedAttribute>())
                continue;
            ValidateConstructorHasNoUnnamedArguments(constructor);
            ValidateConstructorHasNoMismatchedNamedArguments(constructor);
        }
    }

    void ConstructibleItem::ValidateConstructorHasNoUnnamedArguments(Members::MemberPointer constructor) const
    {
        for (size_t i = 0; i < constructor->GetArity(); ++i)
        {
            if (constructor->GetArgumentInfo(i)->GetName().empty())
            {
                std::stringstream sstream;
                sstream << "Constructor registration is incorrect: Argument " << i << " of " << m_type->GetName() << FormatSignature(*constructor, m_typeLibrary) << " is unnamed" << std::endl;
                sstream << "You can fix this in your class where you registering your constructor:" << std::endl;
                sstream << "cls.Constructor" << FormatSignature(*constructor, m_typeLibrary, "<>", false) << "()" << std::endl;
                sstream << "   .AddSignature(";
                for (size_t i = 0; i < constructor->GetArity(); ++i)
                {
                    if (i > 0)
                        sstream << ", ";
                    if (constructor->GetArgumentInfo(i)->GetName().empty())
                        sstream << "\"<Argument name " << i << ">\"";
                    else
                        sstream << "\"" << constructor->GetArgumentInfo(i)->GetName() << "\"";
                }                    
                sstream << ")";
                throw std::logic_error(sstream.str());
            }
        }
    }

    void ConstructibleItem::ValidateConstructorHasNoMismatchedNamedArguments(Members::MemberPointer constructor) const
    {
        for (size_t i = 0; i < constructor->GetArity(); ++i)
        {
            if (!constructor->GetArgumentInfo(i)->GetName().empty())
            {
                Members::MemberPointer member = m_type->Lookup(constructor->GetArgumentInfo(i)->GetName());
                if (!member)
                {
                    std::stringstream sstream;
                    sstream << "Constructor registration is incorrect: Argument " << i << ", '" << constructor->GetArgumentInfo(i)->GetName() << "' of " << m_type->GetName() << FormatSignature(*constructor, m_typeLibrary) << " does not match any of the named properties in the class";
                    throw std::logic_error(sstream.str());
                }
                else if(member->GetMemberType() != Members::MemberType::TypePropertyGetSet)
                {
                    std::stringstream sstream;
                    sstream << "Constructor registration is incorrect: Argument " << i << ", '" << constructor->GetArgumentInfo(i)->GetName() << "' of " << m_type->GetName() << FormatSignature(*constructor, m_typeLibrary) << " represents a " << member->GetMemberType().ToString() << ". Only Read/Write properties (defined using SetGet(...)) can be matched with constructor arguments";
                    throw std::logic_error(sstream.str());
                }
            }
        }
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ConstructibleItem**)
    {
        using namespace Classes;
        Class<ConstructibleItem> cls(typeLibrary, "ConstructibleItem");
        cls.AddAttribute<ValidationAttribute>([](const ConstructibleItem& item) {item.Validate(); },"");
        RegisterToStringFunction(cls);
    }

    std::string ToString(const ConstructibleItem& item, const DNVS::MoFa::Formatting::FormattingService& formattingService)
    {
        if (!item.GetType())
            return "New unknown type";
        else
            return "New " + item.GetType()->GetName();
    }

}}}}


