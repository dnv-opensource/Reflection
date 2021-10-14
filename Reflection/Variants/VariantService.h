#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Types/TypeTraits.h"

#include "Reflection/Variants/StackStorage.h"
#include "Reflection/Variants/ConversionError.h"
#include "Reflection/Variants/Variant.h"

#include "InternalVariantService.h"


namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    /*
    The variant service will take any c++ type and create a Variant from it with the correct DecoratedTypeInfo set.
    Under the hood, it will use the InternalVariantService. The difference is that the InternalVariantService doesn't modify the type of the variant 
    nor check the data before converting, while the VariantService will do type checking and will set the type during creation of a variant.
    */
    class VariantService 
    {
    public:

        template<typename T>
        static Variant ReflectDynamic(T* value, typename std::enable_if_t<std::is_polymorphic_v<T>,void*> = 0)
        {
            if (value)
            {
                Variant result(dynamic_cast<void*>(value), Types::DecoratedTypeInfo(typeid(*value), Types::DecoratedTypeDeducer<T*>::value));
                return result;
            }
            else
                return ReflectType<T*>(value);
        }
        template<typename T>
        static Variant ReflectDynamic(const T& value, typename std::enable_if_t<std::is_polymorphic_v<T>, void*> = 0)
        {
            Variant result(dynamic_cast<void*>(const_cast<T*>(&value)), Types::DecoratedTypeInfo(typeid(value), Types::DecoratedTypeDeducer<const T&>::value));
            return result;
        }
        template<typename T>
        static Variant ReflectDynamic(T* value, typename std::enable_if_t<!std::is_polymorphic_v<T>, void*> = 0)
        {
            return ReflectType<T*>(value);
        }
        template<typename T>
        static Variant ReflectDynamic(const T& value, typename std::enable_if_t<!std::is_polymorphic_v<T>, void*> = 0)
        {
            return ReflectType<const T&>(value);
        }        /**
        Reflects any type T into a Variant. Uses the InternalVariantService. More details found there.
        The type T is expected to be specified as part of the function call 
        Example: VariantService::ReflectType<int>(5);
        */
        template<typename T>
        static Variant ReflectType(T value) 
        {
            Variant result(nullptr, Types::TypeId<T>());
            InternalVariantService::SetVariantData<T>(result, value);
            return result;
        }

        ///Fallback when reflecting a variable.
        template<typename T>
        static Variant ReflectType(const Variant& variable) 
        {
            return variable;
        }

        ///Fallback when reflecting a variable
        static const Variant& Reflect(const Variant& var) {return var; }
        static const Variant& Reflect(Variant& var) { return var; }

        template<typename T>
        static Variant Reflect(T&& value)
        {
            Variant result(nullptr, Types::TypeId<T>());
            InternalVariantService::SetVariantData<T>(result, std::forward<T>(value));
            return result;
        }

        ///Unreflects a variable to the given type T. Will not do any conversion, so the variables type must match exactly that of the given type T.
        template<typename T>
        inline static typename Types::ReturnType<T> Unreflect(const Variant& var)
        {
            const Types::DecoratedTypeInfo& info = var.GetDecoratedTypeInfo();
            if (info == Types::TypeId<T>())
            {
                return InternalVariantService::UnreflectUnchecked<T>(var);
            }
            else
            {
                throw ConversionError(info.GetTypeInfo(), Types::TypeId<T>().GetTypeInfo());
            }
        }
    };

}}}}
