//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "EnumParser.h"
#include "Formatting/DefaultFormatterRules.h"
#include "EnumHelper.h"
#include "Reflection/Members/IMember.h"
#include "Reflection/Attributes/AttributeCollection.h"
#include "Reflection/Attributes/SerializationAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {

    EnumParser::EnumParser(const TypeLibraries::TypePointer& type) : m_weakType(type)
    {

    }

    Variants::Variant EnumParser::operator()(const std::string& text, const Formatting::FormattingService& formattingService) const
    {
        using namespace Formatting;
        std::shared_ptr<IFormatterRules> formatterRules = formattingService.GetFormatterOrDefault<IFormatterRules, DefaultFormatterRules>();
        switch (formatterRules->GetExpressionRepresentation())
        {
        case IFormatterRules::Serialization:
            return ParseSerializedMembers(text, formattingService);
        default:
            return Variants::Variant();
        }
    }

    Variants::Variant EnumParser::ParseSerializedMembers(const std::string& text, const Formatting::FormattingService& formattingService) const
    {
        PopulateMembers(m_membersSerialized, formattingService);
        auto it = m_membersSerialized.find(text);
        if (it != m_membersSerialized.end())
            return it->second;
        else
            return Variants::Variant();
    }

    void EnumParser::PopulateMembers(std::map<std::string, Variants::Variant>& members, const Formatting::FormattingService& formattingService) const
    {
        if (m_membersSerialized.empty())
        {
            EnumHelper helper(m_weakType.lock());
            for (auto member : helper.GetAllEnumMembers())
            {
                members[helper.ToString(member, formattingService)] = member->InvokeNative({});
                if (member->GetAttributeCollection().HasAttribute<Attributes::SerializationAttribute>())
                {
                    for (const std::string& alternative : member->GetAttributeCollection().GetAttribute<Attributes::SerializationAttribute>().GetAlternativeNames())
                    {
                        members[alternative] = member->InvokeNative({});
                    }
                }

            }
        }
    }

}}}}

