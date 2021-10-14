//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/BuiltInConversions.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Reflection/TypeConversions/TrivialConversion.h"
#include "Reflection/TypeConversions/ImplicitConversion.h"
#include "LValueConversion.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    ///Defines all the built in conversion between native types (int,double etc)
    class BuiltInConversions {
    public:
        void AddBuiltInConversions(ConversionGraphPointer conversionGraph) {
            AddImplicitConversionTo<unsigned char>(conversionGraph);
            AddImplicitConversionTo<signed char>(conversionGraph);
            AddImplicitConversionTo<unsigned short>(conversionGraph);
            AddImplicitConversionTo<signed short>(conversionGraph);
            AddImplicitConversionTo<unsigned int>(conversionGraph);
            AddImplicitConversionTo<signed int>(conversionGraph);
            AddImplicitConversionTo<unsigned long>(conversionGraph);
            AddImplicitConversionTo<signed long>(conversionGraph);
            AddImplicitConversionTo<unsigned __int64>(conversionGraph);
            AddImplicitConversionTo<signed __int64>(conversionGraph);

            AddImplicitConversionTo<char>(conversionGraph);
            AddImplicitConversionTo<bool>(conversionGraph);
            AddImplicitConversionTo<float>(conversionGraph);
            AddImplicitConversionTo<double>(conversionGraph);
            AddImplicitConversionTo<long double>(conversionGraph);
        }
    private:
        template<typename From,typename To>
        struct ImplicitConversionAdder {
            static void Add(const ConversionGraphPointer& conversionGraph) {
                AddImplicitConversion<From,To>(conversionGraph);
				AddImplicitConversion<From, const To>(conversionGraph);
			}
        };
        template<typename From>
        struct ImplicitConversionAdder<From,From> {
            static void Add(const ConversionGraphPointer& conversionGraph) {
            }
        };
        template<typename T>
        void AddImplicitConversionTo(const ConversionGraphPointer& conversionGraph) {
            Types::TypeTag<T> tag;
            AddLValueConversionImpl<T>(conversionGraph, tag);
            ImplicitConversionAdder<unsigned char,T>::Add(conversionGraph);
            ImplicitConversionAdder<signed char,T>::Add(conversionGraph);
            ImplicitConversionAdder<unsigned short,T>::Add(conversionGraph);
            ImplicitConversionAdder<signed short,T>::Add(conversionGraph);
            ImplicitConversionAdder<unsigned int,T>::Add(conversionGraph);
            ImplicitConversionAdder<signed int,T>::Add(conversionGraph);
            ImplicitConversionAdder<unsigned long,T>::Add(conversionGraph);
            ImplicitConversionAdder<signed long,T>::Add(conversionGraph);
            ImplicitConversionAdder<unsigned __int64,T>::Add(conversionGraph);
            ImplicitConversionAdder<__int64,T>::Add(conversionGraph);

            ImplicitConversionAdder<char,T>::Add(conversionGraph);
            ImplicitConversionAdder<bool,T>::Add(conversionGraph);
            ImplicitConversionAdder<float,T>::Add(conversionGraph);
            ImplicitConversionAdder<double,T>::Add(conversionGraph);
            ImplicitConversionAdder<long double,T>::Add(conversionGraph);
            AddTrivialConversions<T>(conversionGraph);
        }
    };

    void AddBuiltInConversions(ConversionGraphPointer conversionGraph) {
        BuiltInConversions().AddBuiltInConversions(conversionGraph);
    }
}}}}