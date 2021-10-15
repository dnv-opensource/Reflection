#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsStack.h"
#include "Reflection\TypeConversions\TypeConversionService.h"
#include "Reflection\Variants\VariantService.h"

std::string JSSCRIPT_IMPORT_EXPORT FormatType(const DNVS::MoFa::Reflection::Types::DecoratedTypeInfo& type);

template<typename T, typename EnableIf = void>
struct jsReflecter {
    static DNVS::MoFa::Reflection::Variants::Variant Reflect(T from)
    {
        return DNVS::MoFa::Reflection::Variants::VariantService::ReflectType<T>(from);
    }
};

template<typename T>
struct jsReflecter<T*, std::enable_if_t<!(std::is_base_of_v<T,jsValue> || std::is_same_v<T,jsValue>)>>
{
    static DNVS::MoFa::Reflection::Variants::Variant Reflect(T* from)
    {
        return DNVS::MoFa::Reflection::Variants::VariantService::ReflectDynamic(from);
    }
};

template<typename From,typename To>
class jsTypeConverter
{
public:
    jsTypeConverter(jsStack* stack) 
        :   m_stack(stack)
    {
    }

    bool IsInitialized() const
    {
        return m_stack != 0 && m_stack->GetConversionGraph();
    }

    To Convert(From from)
    {
        using namespace DNVS::MoFa::Reflection;

        TypeConversions::TypeConversionService service(m_stack->GetConversionGraph());
        Variants::Variant originalValue = jsReflecter<From>::Reflect(from);

        try
        {
            TypeConversions::ConversionSequencePointer conversionSequence = service.GetConversionSequence<To>(originalValue);

            if(conversionSequence && conversionSequence->IsValid())
            {
                conversionSequence->IntrusiveConvert(originalValue);
                jsStack::stack()->insert(originalValue);
                return Variants::InternalVariantService::UnreflectUnchecked<To>(originalValue);
            }
        }
        catch(std::exception)
        {
        }

        throw bad_type_conversion(FormatType(originalValue.GetDecoratedTypeInfo()),FormatType(Types::TypeId<To>()));
    }
private:
    jsStack* m_stack;
};
