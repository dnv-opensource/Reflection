//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IMember_Reflection.h"
#include "../Members/IMember.h"
#include "../Classes/Class.h"
#include "../Attributes/NameAttribute.h"
#include "../Attributes/BindingAttribute.h"
#include "IAttribute_Reflection.h"
#include "../Utilities/MemberItem.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    using namespace Classes;

    std::string ToString(const MemberPointer& member, const Formatting::FormattingService& formattingService)
    {
        return ToString(Utilities::MemberItem(member, Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>()), formattingService);
    }
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, ArgumentInfo**)
    {
        Class<ArgumentInfo, ArgumentInfoPointer, NoDynamicTypeDeduction> cls(typeLibrary, "ArgumentInfo");
        cls.Function("AddAttribute", [](ArgumentInfoPointer argument, std::shared_ptr<IAttribute> attribute) {
            argument->GetAttributeCollection().AddAttribute(attribute);
            });
        cls.Function("RemoveAttribute", [](ArgumentInfoPointer argument, MemberPointer constructor) {
            argument->GetAttributeCollection().RemoveAttribute(constructor->GetReturnType().GetTypeInfo());
            });
    }
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, IMember**)
    {
        Reflect<ArgumentInfo>(typeLibrary);
        Attributes::ReflectAttributes(typeLibrary);
        Class<IMember, MemberPointer, NoDynamicTypeDeduction> cls(typeLibrary, "IMember");
        cls.AddAttribute<NameAttribute>([](const MemberPointer& member) {return member->GetName(); }, [](MemberPointer& member, const std::string& name) {member->SetName(name); });
        cls.Operator((*This)[Other<std::string>()], [](MemberPointer member, const std::string& name) -> ArgumentInfoPointer {
            for (size_t i = 0; i < member->GetArity(); ++i)
            {
                if (_stricmp(member->GetArgumentInfo(i)->GetName().c_str(), name.c_str()) == 0)
                    return member->GetArgumentInfo(i);
            }
            throw std::runtime_error("Invalid argument name: " + name);
        });
        cls.Function("AddAttribute", [](MemberPointer member, std::shared_ptr<IAttribute> attribute) {
            member->GetAttributeCollectionCreateIfMissing().AddAttribute(attribute);
        });
        cls.Function("RemoveAttribute", [](MemberPointer member, MemberPointer constructor) {
            member->GetAttributeCollectionCreateIfMissing().RemoveAttribute(constructor->GetReturnType().GetTypeInfo());
        });
        cls.Function("ToString", ToString).AddSignature(Arg("formattingService") = Formatting::FormattingService());
    }
}}}}

