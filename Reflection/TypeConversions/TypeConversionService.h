#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/IConversionGraph.h"

#include "Reflection/Variants/Variant.h"
#include "Reflection/Variants/VariantService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    class TypeConversionService {
    public:
        TypeConversionService(ConversionGraphPointer conversionGraph) : m_conversionGraph(conversionGraph) {}
        /**
        Tries to convert input to a variable of type T by supplying valid conversions registered in the conversion graph.        
        Use with caution when converting from a reference variable to a stack variable. The stack variable will go out of scope.
        */
        template<typename T>
        Types::ReturnType<T> As(const Variants::Variant& input) const {
            ConversionSequencePointer sequence=GetConversionSequence<T>(input);
            if(sequence && sequence->IsValid()) {
                Variants::Variant result(input);
                sequence->IntrusiveConvert(result);
                return Variants::InternalVariantService::UnreflectUnchecked<T>(result);
            }
            else 
            {
                throw Variants::ConversionError(input.GetDecoratedTypeInfo().GetTypeInfo(),Types::TypeId<T>().GetTypeInfo());
            }
        }
        ///Tries to convert input to variable of type T. Stores this in input and tries to unreflect input to T.
        template<typename T>
        Types::ReturnType<T> IntrusiveAs(Variants::Variant& input) const 
        {
            ConversionSequencePointer sequence=GetConversionSequence<T>(input);
            if(sequence && sequence->IsValid()) {
                sequence->IntrusiveConvert(input);
                return Variants::InternalVariantService::UnreflectUnchecked<T>(input);
            }
            else 
            {
                throw Variants::ConversionError(input.GetDecoratedTypeInfo().GetTypeInfo(),Types::TypeId<T>().GetTypeInfo());
            }
        }
        template<typename T>
        bool IsConvertibleTo(const Variants::Variant& input) const {
            ConversionSequencePointer sequence=GetConversionSequence<T>(input);
            return sequence && sequence->IsValid();
        }
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& info, const Variants::Variant& input) const {
            ConversionSequencePointer sequence = m_conversionGraph->GetConversionSequence(input, info);
            return sequence && sequence->IsValid();
        }

        template<typename T>
        ConversionSequencePointer GetConversionSequence(const Variants::Variant& input) const {
            return m_conversionGraph->GetConversionSequence(input,Types::TypeId<T>());
        }
        ConversionGraphPointer GetConversionGraph() { return m_conversionGraph; }
    private:
        ConversionGraphPointer m_conversionGraph;
    };
}}}}