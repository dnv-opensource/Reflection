//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Delegate.h"
#include "Reflection/Members/MemberWithPredefinedFirstArgument.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {

    Delegate::Delegate(const Object& object, const std::string& member, Members::MemberType type) 
        : m_object(object)
        , m_member(member)
        , m_type(type)
    {

    }
    
    Delegate& Delegate::operator=(const Object& other)
    {
        (*this)(other);
        return *this;
    }

    Members::MemberPointer Delegate::AsMember()
    {
        if (!m_object.GetTypeLibrary())
            return nullptr;
        auto typeLibrary = m_object.GetTypeLibrary();
        TypeLibraries::TypePointer type = m_object.GetType();
        if (!type)
            return nullptr;
        Members::MemberPointer member = type->Lookup(m_member, m_object.GetVariant());
        if (!member)
            return nullptr;
        return new Members::MemberWithPredefinedFirstArgument(member, m_object.GetVariant());
    }

}}}}
