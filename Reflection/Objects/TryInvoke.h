#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Object.h"
#include "Reflection/TypeConversions/TypeConversionService.h"
#include "Reflection/Members/MemberWithArguments.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    /*
    Special class to handle errors from TryInvoke.
    If TryInvoke fails, it will return an empty ObjectOrNone.
    The caller is then responsible for calling either 
    - GetValueOr(<defaultValue>);
    or
    - TryGetValue(outputValue);
    */
    class ObjectOrNone 
    {
    public:
        ObjectOrNone(const Object& object) : m_object(object) {}
        ObjectOrNone() {}
        //Gets the value stored in m_object if it is convertible to T.
        //Otherwise, returns defaultValue,
        template<typename T>
        T GetValueOr(const T& defaultValue) const
        {
            T value;
            if (TryGetValue(value))
                return value;
            else
                return defaultValue;
        }

        //Tries to convert m_object to T and store it in value.
        //If successful, returns true.
        //Otherwise, returns false.
        template<typename T>
        bool TryGetValue(T& value) const
        {
            if (!m_object.IsValid())
                return false;
            if (!m_object.GetTypeLibrary())
                return false;
            TypeConversions::TypeConversionService service(m_object.GetTypeLibrary()->GetConversionGraph());
            auto conversionSequence = service.GetConversionSequence<T>(m_object.GetVariant());
            if (conversionSequence && conversionSequence->IsValid())
            {
                Variants::Variant result(m_object.GetVariant());
                conversionSequence->IntrusiveConvert(result);
                value = Variants::InternalVariantService::UnreflectUnchecked<T>(result);
                return true;
            }
            else
                return false;
        }
    private:
        Object m_object;
    };

    //Special case to handle const Object& (needs no conversion)
    inline Object ConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary, const Object& arg)
    {
        return arg;
    }
    //Special case to handle Object& (needs no conversion)
    inline Object ConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary, Object& arg)
    {
        return arg;
    }

    //Convert T to object.
    template<typename T>
    Object ConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary, T&& arg)
    {
        return Object::Create<T>(typeLibrary, std::forward<T>(arg));
    }

    //Convert T*& to T* and converts it to an Object
    template<typename T>
    Object ConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary, T*& arg)
    {
        return Object::Create<T*>(m_object.GetTypeLibrary(), arg);
    }

    //Convert const char[N] to const char* and converts it to an Object
    template<typename T, int N>
    Object ConvertToObject(const TypeLibraries::TypeLibraryPointer& typeLibrary, const T(&arg)[N])
    {
        return Object::Create<const T*>(typeLibrary, (const T*)(arg));
    }

    //Tries to invoke the method <methodName> on object with the supplied arguments <args>.
    //First, all the arguments are converted to Object.
    template<typename... Args>
    ObjectOrNone TryInvoke(const Object& obj, const std::string& methodName, Args&&... args)
    {
        std::vector<Object> objects{ ConvertToObject<Args>(obj.GetTypeLibrary(), args)... };
        auto method = obj.PrepareInvoke(methodName, objects);
        if (!method || !method->IsOk())
            return ObjectOrNone();
        return Object(obj.GetTypeLibrary(), method->Invoke());
    }
}}}}