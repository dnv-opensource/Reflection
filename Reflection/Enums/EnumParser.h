#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting/FormattingService.h"
#include "Reflection/Config.h"
#include "Reflection/TypeLibraries/IType.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {
    /*
    This class will accept an enum type and store a map between the string representation of the enum and the actual enum value.
    Currently only the serialized string value is supported.
    This is used when generating XML from reflection information.
    */
    class REFLECTION_IMPORT_EXPORT EnumParser {
    public:
        EnumParser(const TypeLibraries::TypePointer& type);
        Variants::Variant operator()(const std::string& text, const DNVS::MoFa::Formatting::FormattingService& formattingService) const;
    private:
        Variants::Variant ParseSerializedMembers(const std::string& text, const DNVS::MoFa::Formatting::FormattingService& formattingService) const;
        std::weak_ptr<TypeLibraries::IType> m_weakType;
        mutable std::map<std::string, Variants::Variant> m_membersSerialized;
        void PopulateMembers(std::map<std::string, Variants::Variant>& members, const DNVS::MoFa::Formatting::FormattingService& formattingService) const;
    };
}}}}