#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeTraits.h"
#include "DecoratedTypeDeducer.h"
#include "DeduceTypeTag.h"
#include <typeindex>
#include <type_traits>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    /**
    Uniquely defines the type info of any C++ type.
    The type is split into two: One for the actual type and one for the decoration of the type:
    E.g. 
    double& will be split into double and int(Reference) <- The int is used as a vessel to carry the decoration.
    scBeam* will be split into scBeam and int(Pointer)
    */
    class DecoratedTypeInfo
    {
    public:
        DecoratedTypeInfo() : m_info(0), m_decoration(-1){}
        DecoratedTypeInfo(const DecoratedTypeInfo& other) 
            :   m_info(other.m_info)
            ,   m_decoration(other.m_decoration)
        {}
        ///Allow DecoratedTypeInfo to represent managed types. 
        ///Then decoration will be TypeDecoration::ManagedHandle and info will represent the handle IntPtr for the RuntimeTypeHandle.
        DecoratedTypeInfo(const void* info, int decoration)
            : m_info(info)
            , m_decoration(decoration)
        {}

        DecoratedTypeInfo(const std::type_info& info, int decoration=0) 
            :   m_info(&info)
            ,   m_decoration(decoration) 
        {}
        DecoratedTypeInfo& operator=(const DecoratedTypeInfo& other) 
        {
            m_decoration = other.m_decoration;
            m_info = other.m_info;
            return *this;
        }
        inline bool operator<(DecoratedTypeInfo const& rhs) const 
        {
            if(m_decoration != rhs.m_decoration)
                return m_decoration < rhs.GetDecoration();
            else if (m_decoration < 0)
                return m_info < rhs.m_info;
            else 
                return GetTypeInfoPointer()->before(*rhs.GetTypeInfoPointer()) != 0;
        }
        inline bool operator==(DecoratedTypeInfo const& rhs) const
        {
            if (m_decoration != rhs.m_decoration)
                return false;
            else if (m_info == rhs.m_info)
                return true;
            else if (m_decoration < 0)
                return m_info != rhs.m_info;
            else if(m_info && rhs.m_info)
                return *GetTypeInfoPointer() == *rhs.GetTypeInfoPointer();
            else
                return false;
            return true;
        }
        inline bool operator<=(DecoratedTypeInfo const& rhs) const { return !(rhs<*this); }
        inline bool operator>=(DecoratedTypeInfo const& rhs) const { return !(*this<rhs); }
        inline bool operator>(DecoratedTypeInfo const& rhs) const { return rhs < *this; }
        inline bool operator!=(DecoratedTypeInfo const& rhs) const { return !(rhs == *this); }

        const std::type_info* GetTypeInfoPointer() const 
        {
            return static_cast<const std::type_info*>(m_info);
        }
        std::type_index GetTypeInfo() const 
        {
            if (m_decoration >= 0  && m_info)
            {
                return *GetTypeInfoPointer();
            }
            else
            {
                return std::type_index(typeid(void));
            }
        }
        int GetDecoration() const {return m_decoration; }
		bool IsValid() const { return m_info != nullptr; }
    private:
        //Defines the primitive type, or if m_decoration is -1, the managed type handle.
        const void* m_info;
        //Defines const,volatile,* and &
        int m_decoration;
        //const std::type_info* m_decoration;
    };
    template<typename _Elem, typename _Traits>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const DecoratedTypeInfo& info)
    {        
        return _Ostr << info.GetTypeInfo().name();
    }

    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(AnyTag)
    {
        static DecoratedTypeInfo info(typeid(DecoratedTypeDeducer<T>::Type), DecoratedTypeDeducer<T>::value);
        return info;
    }
    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(ValueTag)
    {
        static DecoratedTypeInfo info(typeid(DecoratedTypeDeducer<const T&>::Type), DecoratedTypeDeducer<const T&>::value);
        return info;
    }

    template<typename T>
    const DecoratedTypeInfo& TypeIdImpl(SmartPointerTag)
    {
        typedef typename DeduceTypeTag<T>::PointerType PointerType;
        static DecoratedTypeInfo info(typeid(DecoratedTypeDeducer<PointerType>::Type), DecoratedTypeDeducer<PointerType>::value);
        return info;
    }

    template<typename T>
    const DecoratedTypeInfo& TypeId()
    {
        TypeTag<T> tag;
        return TypeIdImpl<DeducedType<T>>(tag);
    }

}}}}
namespace std {
    template<typename _Elem, typename _Traits>
    std::basic_ostream<_Elem, _Traits>& operator<<(std::basic_ostream<_Elem, _Traits>& _Ostr, const std::type_index& info)
    {        
        return _Ostr << info.name();
    }
}
