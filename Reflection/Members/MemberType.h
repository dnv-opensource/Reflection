#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa { namespace Reflection { namespace Members {
    struct MemberType 
    {
        enum Value 
        {
            TypeNone = 0,
            TypeConstructor = 1,
            TypeMemberFunction = 2,
            TypeStaticFunction = 4,
            TypeFunction = 6,
            TypePropertyGet = 8,
            TypePropertySet = 16,
            TypePropertyGetSet = TypePropertyGet | TypePropertySet,
            TypeOperator = 32,
            TypeAll = TypeConstructor | TypeFunction | TypePropertyGetSet | TypeOperator
        };
        MemberType& operator|=(const MemberType& other) {m_value = Value(m_value | other.m_value); return *this; }
        MemberType(Value value) : m_value(value) {}
        MemberType() : m_value(TypeNone) {}
        MemberType(const MemberType& other) :m_value(other.m_value) {}
        MemberType& operator=(const MemberType& other) {m_value = other.m_value; return *this; }
        operator const Value() const {return m_value; }
        std::string ToString() const 
        {
            switch(m_value) 
            {
            case TypeConstructor: return "Constructor";
            case TypeMemberFunction: return "Member Function";
            case TypeStaticFunction: return "Static Function";
            case TypeFunction: return "Function";
            case TypePropertyGet: return "Read-Only Property";
            case TypePropertySet: return "Write-Only Property";
            case TypePropertyGetSet: return "Read/Write Property";
            case TypeOperator: return "Operator";
            default:return "";
            }
        }
        bool IsMember() const
        {
            return m_value != TypeStaticFunction && m_value != TypeConstructor;
        }
    private:
        Value m_value;
    };

}}}}