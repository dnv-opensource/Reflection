#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ConversionGraphWithComplexDynamicConversion.h"
#include "jsScript_config.h"

class JSSCRIPT_IMPORT_EXPORT jsScriptConversionGraph : public DNVS::MoFa::Reflection::TypeConversions::ConversionGraphWithComplexDynamicConversion
{
public:

    DNVS::MoFa::Reflection::TypeConversions::ConversionSequencePointer GetConversionSequence(const DNVS::MoFa::Reflection::Variants::Variant& from, const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& to) const override;

};