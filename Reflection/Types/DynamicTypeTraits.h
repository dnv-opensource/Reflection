#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DecoratedTypeInfo.h"
#include <string>
#include "Reflection/Config.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    class IConversionGraph;
}}}}
namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {
    inline bool IsConst(const DecoratedTypeInfo& info)
    {
        return (info.GetDecoration() & TypeDecoration::Const) == TypeDecoration::Const;
    }
    inline bool IsPointer(const DecoratedTypeInfo& info)
    {
        return (info.GetDecoration() & TypeDecoration::Pointer) == TypeDecoration::Pointer;
    }
    inline bool IsIntegral(const DecoratedTypeInfo& info)
    {
        return info.GetTypeInfo() == typeid(char) ||
            info.GetTypeInfo() == typeid(short) ||
            info.GetTypeInfo() == typeid(int) ||
            info.GetTypeInfo() == typeid(long) ||
            info.GetTypeInfo() == typeid(unsigned long) ||
            info.GetTypeInfo() == typeid(unsigned int) ||
            info.GetTypeInfo() == typeid(unsigned short) ||
            info.GetTypeInfo() == typeid(unsigned char) ||
            info.GetTypeInfo() == typeid(signed char) ||
            info.GetTypeInfo() == typeid(long long) ||
            info.GetTypeInfo() == typeid(unsigned long long);               
    }
    inline bool IsReference(const DecoratedTypeInfo& info)
    {
        return (info.GetDecoration() & TypeDecoration::Reference) == TypeDecoration::Reference;
    }
    inline bool IsEnum(const DecoratedTypeInfo& info)
    {
        return std::string("enum ").compare(0, 5, info.GetTypeInfo().name(), 5) == 0;
    }
    inline bool IsClass(const DecoratedTypeInfo& info)
    {
        return std::string("class ").compare(0, 6, info.GetTypeInfo().name(), 6) == 0;
    }
    inline bool IsSameAs(const DecoratedTypeInfo& lhs, const DecoratedTypeInfo& rhs)
    {
        return lhs == rhs;
    }
    inline bool IsSameAs(const DecoratedTypeInfo& lhs, const std::type_info& rhs)
    {
        return lhs.GetTypeInfo() == std::type_index(rhs);
    }
    inline bool IsSameAs(const std::type_info& lhs, const DecoratedTypeInfo& rhs)
    {
        return std::type_index(lhs) == rhs.GetTypeInfo();
    }
    inline bool IsNonConstLValue(const DecoratedTypeInfo& rhs)
    {
        if (IsConst(rhs))
            return false;
        if (IsPointer(rhs))
            return true;
        if (IsReference(rhs))
            return true;
        return false;
    }
    inline DecoratedTypeInfo RemoveConst(const DecoratedTypeInfo& lhs)
    {
        return DecoratedTypeInfo(lhs.GetTypeInfoPointer(), lhs.GetDecoration()&~TypeDecoration::Const);
    }
    inline DecoratedTypeInfo AddConst(const DecoratedTypeInfo& lhs)
    {
        return DecoratedTypeInfo(lhs.GetTypeInfoPointer(), lhs.GetDecoration() | TypeDecoration::Const);
    }

    bool REFLECTION_IMPORT_EXPORT IsBaseOf(const std::shared_ptr<TypeConversions::IConversionGraph>& conversionGraph, const DecoratedTypeInfo& base, const DecoratedTypeInfo& derived);
}}}}