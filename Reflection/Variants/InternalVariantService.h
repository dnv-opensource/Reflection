#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Types/TypeTraits.h"
#include "Reflection/Types/DeduceTypeTag.h"

#include "Reflection/Variants/StackStorage.h"
#include "Reflection/Variants/ConversionError.h"
#include "Reflection/Variants/Variant.h"
#include "Reflection/Variants/SmartPointers/AliasingSmartPointerStorage.h"
#include "Reflection/Variants/SmartPointers/CopyableSmartPointerStorage.h"
#include "Reflection/Variants/SmartPointers/MovableSmartPointerStorage.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace Types {   
    struct VariantTag : public AnyTag {};
    template<>
    struct DeduceTypeTag<Variants::Variant, void>
    {
        typedef Variants::Variant Type;
        typedef const Variants::Variant& ArgumentType;
        typedef Variants::Variant ReturnType;
        typedef VariantTag Tag;
    };

    template<>
    struct DeduceTypeTag<const Variants::Variant&, void>
    {
        typedef const Variants::Variant& Type;
        typedef const Variants::Variant& ArgumentType;
        typedef Variants::Variant ReturnType;
        typedef VariantTag Tag;
    };
}}}}

namespace DNVS {namespace MoFa { namespace Reflection { namespace Variants {
    /*
    InternalVariantService is a set of classes that helps in wrapping a C++ type into a Variant.
    A Variant is a representation of a C++ value that does not hold any compile time information about the type.
    Depending on the type you want to wrap, there are different methods we need to use.
    We use the Types::TypeTag<T> overloading in order to find the correct method.
    */
    struct InternalVariantService 
    {
        template<typename T>
        static Types::ReturnType<T> UnreflectUnchecked(const Variant& var)
        {
            Types::TypeTag<T> tag;
            return UnreflectUncheckedImpl<Types::ReturnType<T>>(var, tag);
        }
        template<typename T, typename U>
        static void SetVariantData(Variant& result, U&& argument)
        {
            Types::TypeTag<T> tag;
            return SetVariantDataImpl<Types::ReturnType<T>>(result, std::forward<U>(argument), tag);
        }
    private:
        //Set variant data implementation

        //Unchecked Reflection for pointers
        template<typename T>
        static void SetVariantDataImpl(Variant& result, T pointer, Types::PointerTag)
        {
            typedef typename Types::DecoratedTypeDeducer<T>::Type NakedType;
            result.SetData(static_cast<void*>(const_cast<NakedType*>(pointer)));
        }

        //Unchecked Reflection for references (handled as pointers)
        template<typename T>
        static void SetVariantDataImpl(Variant& result, T reference, Types::ReferenceTag)
        {
            typedef std::remove_const_t<typename Types::DeduceTypeTag<T>::NakedType> NakedType;
            result.SetData(static_cast<void*>(const_cast<NakedType*>(&reference)));
        }

        /** SetVariantData overload for pod data.
        Sets the Variants data (void*) to reflect the input value.
        if T==int, we can store the int inside a void pointer directly.
        If we have int smallPod=5;
        then void* data=0x00000005 (for 32 bit system)
        So the void* no longer represents a pointer.
        */
        template<typename T>
        static void SetVariantDataImpl(Variant& result, T smallPod, Types::SmallPodTag)
        {
            typedef std::remove_cv_t<T> NakedType;
            union CastUnion {
                NakedType actual;
                void* data;
            };
            //Ensure the initial value is NULL
            CastUnion castUnion;
            castUnion.data = 0;
            //Sets the relevant bits.
            castUnion.actual = smallPod;
            result.SetData(castUnion.data);
        }

        template<typename T>
        static void SetVariantDataImpl(Variant& result, Types::ArgumentType<T> value, Types::AliasingSmartPointerTag) 
        {
            typedef Types::DeducedType<T> SmartPointerType;
            result.SetStorage(StoragePointer(new AliasingSmartPointerSelector<SmartPointerType>(value)));
        }

        //Unchecked Reflection for copyable smart pointers
        template<typename T>
        static void SetVariantDataImpl(Variant& result, Types::ArgumentType<T> value, Types::CopyableSmartPointerTag) 
        {
            typedef Types::DeducedType<T> SmartPointerType;
            result.SetStorage(StoragePointer(new CopyableSmartPointerSelector<SmartPointerType>(value)));
        }

        //Unchecked Reflection for copyable smart pointers
        template<typename T>
        static void SetVariantDataImpl(Variant& result, Types::ArgumentType<T> value, Types::MovableSmartPointerTag) 
        {
            typedef Types::DeducedType<T> SmartPointerType;
            result.SetStorage(StoragePointer(new MovableSmartPointerStorage<SmartPointerType>(std::move(value))));
        }

        template<typename T>
        static void SetVariantDataImpl(Variant& result, Types::ArgumentType<T> value, Types::VariantTag)
        {
            result = value;
        }

        //Unchecked Reflection for values types, such as std::string (but not const std::string&)
        /** SetVariantData overload for value types.
        Sets the Variants data (void*) to reflect the input value.
        We need to copy the input data as it is an rvalue.
        We wrap the input data into StackStorage, and let it manage the translation of the stored pointer to void* (trivial)
        */
        template<typename T>
        static void SetVariantDataImpl(Variant& result, Types::ArgumentType<T> value, Types::ValueTag)
        {
            result.SetStorage(StoragePointer(new StackStorage<T>(value)));
        }

        //Unreflect implementation
        //Unchecked Reflection for pointers
        template<typename T>
        static T UnreflectUncheckedImpl(const Variant& var, Types::PointerTag)
        {
            return static_cast<T>(var.GetData());
        }

        //Unchecked Reflection for references (handled as pointers)
        template<typename T>
        static T UnreflectUncheckedImpl(const Variant& var, Types::ReferenceTag)
        {
            typedef std::remove_const_t<typename Types::DeduceTypeTag<T>::NakedType> NakedType;
            return *static_cast<NakedType*>(var.GetData());
        }

        template<typename T>
        static T UnreflectUncheckedImpl(const Variant& var, Types::SmallPodTag)
        {
            typedef std::remove_cv_t<T> NakedType;
            union CastUnion {
                NakedType actual;
                void* data;
            };
            CastUnion castUnion;
            castUnion.data = const_cast<void*>(var.Data());
            return castUnion.actual;
        }

        template<typename T>
        static Types::ReturnType<T> UnreflectUncheckedImpl(const Variants::Variant& var, Types::AliasingSmartPointerTag)
        {
            typedef Types::DeducedType<T> SmartPointerType;
            typedef typename Types::DeduceTypeTag<T>::PointerType PointerType;
            if (!AliasingSmartPointerSelector<SmartPointerType>::IsConvertible(var.GetStorage()))
                throw ConversionError(var.GetDecoratedTypeInfo().GetTypeInfo(), Types::TypeId<SmartPointerType>().GetTypeInfo());
            PointerType p = UnreflectUnchecked<PointerType>(var);
            return AliasingSmartPointerSelector<SmartPointerType>::RecreateSmartPointer(var.GetStorage(), p);
        }

        //Unchecked unreflect for copyable smart pointers
        template<typename T>
        static Types::ReturnType<T> UnreflectUncheckedImpl(const Variants::Variant& var, Types::CopyableSmartPointerTag)
        {
            typedef Types::DeducedType<T> SmartPointerType;
            typedef typename Types::DeduceTypeTag<T>::PointerType PointerType;
            if (!CopyableSmartPointerSelector<SmartPointerType>::IsConvertible(var.GetStorage()))
                throw ConversionError(var.GetDecoratedTypeInfo().GetTypeInfo(), Types::TypeId<SmartPointerType>().GetTypeInfo());
            PointerType p = UnreflectUnchecked<PointerType>(var);
            return CopyableSmartPointerSelector<SmartPointerType>::RecreateSmartPointer(var.GetStorage(), p);
        }

        //Unchecked unreflect for movablesmart pointers
        template<typename T>
        static Types::ReturnType<T> UnreflectUncheckedImpl(const Variants::Variant& var, Types::MovableSmartPointerTag)
        {
            typedef Types::DeducedType<T> SmartPointerType;
            typedef typename Types::DeduceTypeTag<T>::PointerType PointerType;
            if (!MovableSmartPointerStorage<SmartPointerType>::IsConvertible(var.GetStorage()))
                throw ConversionError(var.GetDecoratedTypeInfo().GetTypeInfo(), Types::TypeId<SmartPointerType>().GetTypeInfo());
            PointerType p = UnreflectUnchecked<PointerType>(var);
            return MovableSmartPointerStorage<SmartPointerType>::RecreateSmartPointer(var.GetStorage(), p);
        }

        template<typename T>
        static Types::ReturnType<T> UnreflectUncheckedImpl(const Variant& var, Types::ValueTag)
        {
            return *static_cast<const T*>(var.GetData());
        }
        template<typename T>
        static const Variant& UnreflectUncheckedImpl(const Variant& var, Types::VariantTag)
        {
            return var;
        }
    };
}}}}