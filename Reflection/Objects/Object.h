#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/Variants/Variant.h"
#include "Reflection/Variants/VariantService.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"
#include "Formatting/FormattingService.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    class Delegate;

    struct TypeSafeBool 
    {
        int DummyMember;
        typedef int TypeSafeBool::*Type;

        static Type Convert(bool value) 
        {
            if (value)
                return &TypeSafeBool::DummyMember;
            else 
                return nullptr;
        }
    };
    /**
    Object is a generic way to represent any C++ value and do operations on those values.
    The values are represented as Variants, so when you want to do operations on them (call functions or use operators), you need to 
    first get the TypePointer from the Variant (using the supplied typeLibrary)
    Then you ask whether that type has the correct method or operator overload and invoke it using reflection methods.
    */
    class REFLECTION_IMPORT_EXPORT Object 
    {
    public:
        template<typename T, typename U>
        static Object Create(TypeLibraries::TypeLibraryPointer typeLibrary, U && arg) 
        {
            Object obj(typeLibrary);
            obj.Reset<T>(std::forward<U>(arg));
            return obj;
        }
        template<typename T>
        static Object CreateDynamic(TypeLibraries::TypeLibraryPointer typeLibrary, T* arg)
        {
            Object obj(typeLibrary, Variants::VariantService().ReflectDynamic(arg));
            return obj;
        }
        template<typename T>
        static Object CreateDynamic(TypeLibraries::TypeLibraryPointer typeLibrary, const T& arg)
        {
            Object obj(typeLibrary, Variants::VariantService().ReflectDynamic(arg));
            return obj;
        }
        Object();
        Object(TypeLibraries::TypeLibraryPointer typeLibrary);
        Object(TypeLibraries::TypeLibraryPointer typeLibrary, const Variants::Variant& var);
        Object(TypeLibraries::TypeLibraryPointer typeLibrary, const Object& obj);

        template<typename T>
        Object(TypeLibraries::TypeLibraryPointer typeLibrary, T data) 
            :   m_typeLibrary(typeLibrary)
            ,   m_variable(Variants::VariantService().ReflectType<T>(data))
        {
        }
        bool HasMember(const std::string& method) const;
        Delegate Lookup(const std::string& method, Members::MemberType type = Members::MemberType::TypeAll) const;
        Object Invoke(const std::string& method, const std::vector<Object>& objects, Members::MemberType type = Members::MemberType::TypeAll) const;

        Members::MemberWithArgumentsPointer PrepareInvoke(const std::string& method, const std::vector<Object>& objects, Members::MemberType type = Members::MemberType::TypeAll) const;
        //Looks for an operator= in the type description of m_variant. If found, this method will be called.
        Object& Assign(const Object& rhs);
        //Same as Reset(rhs);
        Object& operator=(const Object& rhs);
        Object operator+(const Object& rhs) const;
        Object operator-(const Object& rhs) const;
        Object operator*(const Object& rhs) const;
        Object operator/(const Object& rhs) const;
        Object operator^(const Object& rhs) const;
        Object operator<<(const Object& rhs) const;
        Object operator>>(const Object& rhs) const;
        Object operator%(const Object& rhs) const;
        Object operator<(const Object& rhs) const;
        Object operator>(const Object& rhs) const;
        Object operator<=(const Object& rhs) const;
        Object operator>=(const Object& rhs) const;
        Object operator==(const Object& rhs) const;
        Object operator!=(const Object& rhs) const;
        Object operator||(const Object& rhs) const;
        Object operator&&(const Object& rhs) const;
        Object operator&(const Object& rhs) const;
        Object operator|(const Object& rhs) const;
        Object operator<<=(const Object& rhs);
        Object operator>>=(const Object& rhs);
        Object operator*=(const Object& rhs);
        Object operator/=(const Object& rhs);
        Object operator%=(const Object& rhs);
        Object operator+=(const Object& rhs);
        Object operator-=(const Object& rhs);
        Object operator&=(const Object& rhs);
        Object operator|=(const Object& rhs);
        Object operator^=(const Object& rhs);
        Object operator+() const;
        Object operator-() const;
        Object operator*() const;
        Object operator~() const;
        //Object operator&() const;
        Object operator!() const;
        Object operator++();
        Object operator--();
        Object operator++(int);
        Object operator--(int);
        Object operator[](const Object& rhs);

        operator TypeSafeBool::Type() const;


        Object ToLValue() const;

        //Tries to convert this object into an Object of the given type.
        //If it fails, it will return an invalid object.
        Object ConvertTo(const Types::DecoratedTypeInfo& info) const;

        //Template version of the ConvertTo method. For convenience.
        template<typename T>
        Object ConvertTo() const
        {
            return ConvertTo(Types::TypeId<T>());
        }
        
        //Tries to convert this object to the most derived type. If Object represents for example mfMetaObject, 
        //ConvertToDynamicType will return the most derived type, such as scStraightBeam.
        Object ConvertToDynamicType() const;
        
        bool CanWrapValue() const;
        //Tries to wrap the value. Returns itself if no wrapping possible.
        Object WrapValue() const;

        bool CanUnwrapValue() const;
        //Tries to unwrap the value. Returns itself if no unwrapping possible.
        Object UnwrapValue() const;

        //First converts this object to the wanted type. 
        //If successful, casts the result to T.
        //If unsuccesful, throws an exception.
        template<typename T>
        Types::ReturnType<T> As() const
        {
            Object result = ConvertTo(Types::TypeId<T>());
            if(result.IsValid())
                return Variants::InternalVariantService::UnreflectUnchecked<T>(result.m_variable);
            else
                throw Variants::ConversionError(ConvertToDynamicType().GetDecoratedTypeInfo().GetTypeInfo(), Types::TypeId<T>().GetTypeInfo());
        }

        //Checks whether this object is convertible to the type given by info.
        bool IsConvertibleTo(const Types::DecoratedTypeInfo& info) const;
        
        //Checks whether this object is convertible to T.
        template<typename T>
        bool IsConvertibleTo() const 
        {
            return IsConvertibleTo(Types::TypeId<T>());
        }

        //Wrap value into a variant. This makes it possible to represent any value as a variant.
        template<typename T, typename U>
        void Reset(U && value) 
        {
            Reset(Variants::VariantService().ReflectType<T>(value));
        }

        //Replaces m_variant with var.
        void Reset(const Variants::Variant& var);
        //Replaces this object with obj. both m_variant and m_typeLibrary is replaced.
        void Reset(const Object& obj);
        //Clears the object by setting m_variant to empty, making the Object not valid.
        void Reset();
        //Returns the type library, this library knows how to convert between different types, such as between double and int.
        const TypeLibraries::TypeLibraryPointer& GetTypeLibrary() const;
        //Returns the variant, this is the value that actually represents our data.
        const Variants::Variant& GetVariant() const;
        //Returns true if m_variable represents a valid object.
        bool IsValid() const;
        //Returns the reflected type representing this object.
        TypeLibraries::TypePointer GetType() const;
        //Returns the full type info, the type (e.g. double) plus the decoration (e.g. const&)
        Types::DecoratedTypeInfo GetDecoratedTypeInfo() const;
    private:
        std::vector<Variants::Variant> BuildArgumentVector( TypeLibraries::TypePointer type, const std::string& method, const std::vector<Object> &objects ) const;
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        Variants::Variant m_variable;
    };

    ///Streaming support for Point
    template<typename _Elem, typename _Traits>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const Object& obj)
    {        
        return _Ostr << ToString(obj, DNVS::MoFa::Formatting::FormattingService());
    }

    inline void swap(Object& lhs, Object& rhs)
    {
        Object tmp(rhs);
        rhs.Reset(lhs);
        lhs.Reset(tmp);
    }
    void REFLECTION_IMPORT_EXPORT DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, Object**);
    std::string REFLECTION_IMPORT_EXPORT ToString(const Object& object, const DNVS::MoFa::Formatting::FormattingService& formattingService);
}}}}
