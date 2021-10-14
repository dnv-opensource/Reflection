//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <Reflection/TypeConversions/IConversion.h>
#include <Reflection/Variants/VariantService.h>

struct FallbackDummyConverter : public DNVS::MoFa::Reflection::TypeConversions::IConversion
{
    virtual DNVS::MoFa::Reflection::Variants::Variant Convert(const DNVS::MoFa::Reflection::Variants::Variant& other)
    {
        int value = DNVS::MoFa::Reflection::Variants::VariantService::Unreflect<int>(other);
        if(value < 0)
            return DNVS::MoFa::Reflection::Variants::VariantService::ReflectType<double>(value);
        else
            return DNVS::MoFa::Reflection::Variants::VariantService::ReflectType<float>((float)value);
    }
    virtual void IntrusiveConvert(DNVS::MoFa::Reflection::Variants::Variant& variable )
    {
        variable = Convert(variable);
    }
};
