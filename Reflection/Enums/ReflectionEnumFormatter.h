#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting/IEnumFormatter.h"
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Enums {
#pragma warning(push)
#pragma warning(disable:4275)
    //Format enums registered with reflection.
    class REFLECTION_IMPORT_EXPORT ReflectionEnumFormatter : public DNVS::MoFa::Formatting::IEnumFormatter
    {
    public:
        virtual std::string FormatEnum(int enumValue, const std::type_info& enumType, const DNVS::MoFa::Formatting::FormattingService& service) const override;
        virtual bool IsSupported(const std::type_info&) const override;
    };
#pragma warning(pop)
}}}}