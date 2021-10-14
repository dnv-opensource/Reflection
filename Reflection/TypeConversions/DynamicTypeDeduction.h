#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "IConversion.h"
#include <type_traits>
#include <memory>

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {

    // first, the case where T has virtual functions
    template<typename T>
    class DynamicTypeDeduction : public IConversion 
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& input)
        {
            if (!input.IsValid())
                return Variants::Variant();
            Variants::Variant result(input);
            if (input.Data() == nullptr)
                return input;
            T* p = static_cast<T*>(input.Data());            
            if(p == nullptr)
                return input;
            if (auto convertedPointer = dynamic_cast<void*>(p))
              result.SetData(convertedPointer);
            result.SetDecoratedTypeInfo(Types::DecoratedTypeInfo(typeid(*p),input.GetDecoratedTypeInfo().GetDecoration()));
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            variable = Convert(variable);
        }

    };

   
    ///Add dynamic type deduction on static types.
    template<typename T>
    typename std::enable_if<std::is_polymorphic<T>::value>::type AddDynamicTypeDeduction(ConversionGraphPointer conversionGraph) {
        conversionGraph->AddConversion(Types::TypeId<T>(),Types::TypeId<void>(),ConversionType::DynamicTypeConversion,std::make_shared<DynamicTypeDeduction<T>>());
    }

    ///Don't add dynamic type deduction on a static type
    template<typename T>
    typename std::enable_if<!std::is_polymorphic<T>::value>::type AddDynamicTypeDeduction(ConversionGraphPointer conversionGraph) {
    }

}}}}
