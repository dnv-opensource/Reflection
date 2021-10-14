#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>
#include <type_traits>
#include "IConversionGraph.h"
#include "Reflection/Types/DecoratedTypeInfo.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {

    //Conversion between pointer and reference types
    template<typename From,typename To>
    class PointerReferenceConversion : public IConversion {
    public:
    public:
        virtual Variants::Variant Convert(const Variants::Variant& var) {
            Variants::Variant result(var);
            result.SetDecoratedTypeInfo(Types::TypeId<To>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
        }
    };

    template<typename Class>
    typename std::enable_if<std::is_class<Class>::value>::type AddPointerReferenceConversions(ConversionGraphPointer conversionGraph)
    {
        conversionGraph->AddConversion(Types::TypeId<Class*>(),Types::TypeId<Class&>(),ConversionType::TrivialConversion,std::make_shared<PointerReferenceConversion<Class*,Class&>>());
        conversionGraph->AddConversion(Types::TypeId<Class&>(),Types::TypeId<Class*>(),ConversionType::TrivialConversion,std::make_shared<PointerReferenceConversion<Class&,Class*>>());
        conversionGraph->AddConversion(Types::TypeId<const Class*>(),Types::TypeId<const Class&>(),ConversionType::TrivialConversion,std::make_shared<PointerReferenceConversion<const Class*,const Class&>>());
        conversionGraph->AddConversion(Types::TypeId<const Class&>(),Types::TypeId<const Class*>(),ConversionType::TrivialConversion,std::make_shared<PointerReferenceConversion<const Class&,const Class*>>());
    }

    template<typename Class>
    typename std::enable_if<!std::is_class<Class>::value>::type AddPointerReferenceConversions(ConversionGraphPointer conversionGraph)
    {
    }

}}}}