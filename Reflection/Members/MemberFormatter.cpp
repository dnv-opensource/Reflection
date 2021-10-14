//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberFormatter.h"
#include "Reflection/Types/ITypeFormatter.h"
#include "Reflection/TypeLibraries/ITypeLibrary.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {

    std::string FormatSignature(const IMember& member, const TypeLibraries::TypeLibraryPointer& typeLibrary, const char* parenthesis, bool includeNames)
    {
        size_t startIndex = 0;
        switch (member.GetMemberType())
        {
        case MemberType::TypeMemberFunction:
        case MemberType::TypePropertyGet:
        case MemberType::TypePropertySet:
            startIndex = 1;
        }
        char startParenthesis = '(';
        char endParenthesis = ')';
        if (parenthesis != nullptr && parenthesis[0] != 0 && parenthesis[1] != 0)
        {
            startParenthesis = parenthesis[0];
            endParenthesis = parenthesis[1];
        }
        std::string signature;
        signature += startParenthesis;
        for (size_t i = startIndex; i < member.GetArity(); ++i) 
        {
            if (i > startIndex)
                signature += ", ";
            signature += typeLibrary->GetTypeFormatter()->FormatType(member.GetArgumentInfo(i)->GetTypeInfo());
            if (includeNames && !member.GetArgumentInfo(i)->GetName().empty())
                signature += " " + member.GetArgumentInfo(i)->GetName();
        }
        signature += endParenthesis;
        return signature;
    }

}}}}


