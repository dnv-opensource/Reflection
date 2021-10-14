#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Reflection/Variants/VariantService.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The TreatAsBoolAttribute lets you treat an enumeration or another value as a boolean in the context of GUI (In order to represent it as e.g. a check box)
    */
    class TreatAsBoolAttribute
    {
    public:
        template<typename TrueType, typename FalseType>
        TreatAsBoolAttribute(TrueType trueValue, FalseType falseValue)
            : m_trueValue(Variants::VariantService::ReflectType(trueValue))
            , m_falseValue(Variants::VariantService::ReflectType(falseValue))
        {}
        const Variants::Variant& GetTrueValue() const { return m_trueValue; }
        const Variants::Variant& GetFalseValue() const { return m_falseValue; }
    private:
        Variants::Variant m_trueValue;
        Variants::Variant m_falseValue;
    };
}}}}