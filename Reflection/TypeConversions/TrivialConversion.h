#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/IConversion.h"
#include "Reflection/TypeConversions/IConversionGraph.h"

#include "Reflection/Variants/StackStorage.h"
#include "Reflection/Variants/InternalVariantService.h"

#include "Reflection/Types/TypeTraits.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    template<typename From, typename To, typename EnableIf = void>
    class TrivialConversion;

    ///Conversion from non-const to const pointer.
    template<typename From>
    class TrivialConversion<From*, const From*> : public IConversion
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            result.SetDecoratedTypeInfo(Types::TypeId<const From*>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
        }
    };
    template<typename From>
    class TrivialConversion<const From, From> : public IConversion
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            result.SetDecoratedTypeInfo(Types::TypeId<From>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
        }
    };

    ///Conversion from non-const to const reference.
    template<typename From>
    class TrivialConversion<From&, const From&> : public IConversion
    {
    public:
        virtual Variants::Variant Convert( const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            result.SetDecoratedTypeInfo(Types::TypeId<const From&>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
        }
    };

    ///Conversion from value to const reference. Only supported for small pod types that fit in a sizeof(void*).
    template<typename From>
    class TrivialConversion<From, const From&, typename std::enable_if<Types::IsSmallPodType<From>::value>::type> : public IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<const From&>());
            return result;
        }
        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            From from = Variants::InternalVariantService::UnreflectUnchecked<From>(variable);
            variable.SetStorage(Variants::StoragePointer(new Variants::StackStorage<From>(from)));
        }
    };

    ///Conversion from const reference to value. Only supported for small pod types that fit in a sizeof(void*).
    template<typename From>
    class TrivialConversion<const From&, From, typename std::enable_if<Types::IsSmallPodType<From>::value>::type> : public IConversion
    {
    public:
        virtual Variants::Variant Convert(const Variants::Variant& var) 
        {
            Variants::Variant result(var);
            IntrusiveConvert(result);
            result.SetDecoratedTypeInfo(Types::TypeId<From>());
            return result;
        }

        virtual void IntrusiveConvert( Variants::Variant& variable ) 
        {
            const From& from = Variants::InternalVariantService::UnreflectUnchecked<const From&>(variable);
            Variants::InternalVariantService::SetVariantData<From>(variable, from);
        }

    };

    ///Add trivial conversions for small pod types. These types can have a type that is the value type.
    template<typename T>
    typename std::enable_if<Types::IsSmallPodType<T>::value>::type AddTrivialConversions(ConversionGraphPointer conversionGraph) 
    {
        conversionGraph->AddConversion(Types::TypeId<T*>(), Types::TypeId<const T*>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<T*, const T*>>());
        conversionGraph->AddConversion(Types::TypeId<T&>(), Types::TypeId<const T&>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<T&, const T&>>());
        conversionGraph->AddConversion(Types::TypeId<T>(), Types::TypeId<const T&>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<T, const T&>>());
        conversionGraph->AddConversion(Types::TypeId<const T&>(), Types::TypeId<T>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<const T&, T>>());
        conversionGraph->AddConversion(Types::TypeId<const T>(), Types::TypeId<T>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<const T, T>>());
    }

    ///Add trivial conversions for all other types. All value types will always be treated as if they are const reference.
    template<typename T>
    typename std::enable_if<!Types::IsSmallPodType<T>::value>::type AddTrivialConversions(ConversionGraphPointer conversionGraph) 
    {
        conversionGraph->AddConversion(Types::TypeId<T*>(), Types::TypeId<const T*>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<T*, const T*>>());
        conversionGraph->AddConversion(Types::TypeId<T&>(), Types::TypeId<const T&>(), ConversionType::TrivialConversion, std::make_shared<TrivialConversion<T&, const T&>>());
    }

}}}}