//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute_Reflection.h"
#include "../Attributes/BindingAttribute.h"
#include "../Attributes/BindToSelectionAttribute.h"
#include "../Attributes/CaptionAttribute.h"
#include "../Attributes/TabAttribute.h"
#include "../Enums/Enum.h"
#include "../Attributes/GroupAttribute.h"
#include "../Attributes/VisibleAttribute.h"
#include "../Attributes/DefaultAttribute.h"
#include "../Attributes/AllowNullAttribute.h"
#include "../Attributes/AllowCreateAndModifyAttribute.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    using namespace Classes;
    using namespace Enums;
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, IAttribute**)
    {
        Class<IAttribute, std::shared_ptr<IAttribute>> cls(typeLibrary, "IAttribute");
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BindingAttribute**)
    {
        Class<BindingAttribute, Public<IAttribute>, std::shared_ptr<BindingAttribute>> cls(typeLibrary, "BindingAttribute");
        std::weak_ptr<TypeLibraries::ITypeLibrary> weakLibrary(typeLibrary);
        cls.Constructor([weakLibrary](Members::MemberPointer member) 
        {
            return std::make_shared<BindingAttribute>(weakLibrary.lock(), member);
        }).AddSignature("bindingFunction");
    }
    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, VisibleAttribute**)
    {
        Class<VisibleAttribute, Public<IAttribute>, std::shared_ptr<VisibleAttribute>> cls(typeLibrary, "VisibleAttribute");
        std::weak_ptr<TypeLibraries::ITypeLibrary> weakLibrary(typeLibrary);
        cls.Constructor([weakLibrary](Members::MemberPointer member){
            return std::make_shared<VisibleAttribute>(weakLibrary.lock(), member);
        }).AddSignature("visibleFunction");
        cls.Constructor<bool>().AddSignature("isVisible");
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, DefaultAttribute**)
    {
        Class<DefaultAttribute, Public<IAttribute>, std::shared_ptr<DefaultAttribute>> cls(typeLibrary, "DefaultAttribute");
        std::weak_ptr<TypeLibraries::ITypeLibrary> weakLibrary(typeLibrary);
        cls.Constructor([weakLibrary](Members::MemberPointer member) {
            return std::make_shared<DefaultAttribute>(weakLibrary.lock(), member);
        }).AddSignature("defaultFunction");
        cls.Constructor<Variants::Variant>().AddSignature("defaultValue");
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, AllowCreateAndModifyAttribute**)
    {
        Class<AllowCreateAndModifyAttribute, Public<IAttribute>, std::shared_ptr<AllowCreateAndModifyAttribute>> cls(typeLibrary, "AllowCreateAndModifyAttribute");
        std::weak_ptr<TypeLibraries::ITypeLibrary> weakLibrary(typeLibrary);
        cls.Constructor();
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, AllowNullAttribute**)
    {
        Class<AllowNullAttribute, Public<IAttribute>, std::shared_ptr<AllowNullAttribute>> cls(typeLibrary, "AllowNullAttribute");
        std::weak_ptr<TypeLibraries::ITypeLibrary> weakLibrary(typeLibrary);
        cls.Constructor();
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, CaptionAttribute**)
    {
        Class<CaptionAttribute, Public<IAttribute>, std::shared_ptr<CaptionAttribute>> cls(typeLibrary, "CaptionAttribute");
        cls.Constructor<const std::string&>();
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, BindToSelectionAttribute**)
    {
        Class<BindToSelectionAttribute, Public<IAttribute>, std::shared_ptr<BindToSelectionAttribute>> cls(typeLibrary, "BindToSelectionAttribute");
        cls.Constructor();
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, GroupAttribute**)
    {
        Class<GroupAttribute, Public<IAttribute>, std::shared_ptr<GroupAttribute>> cls(typeLibrary, "GroupAttribute");
        cls.Constructor<const std::string&, const std::string&>().AddSignature("groupName", Arg("documentation") = std::string());
    }

    void DoReflect(TypeLibraries::TypeLibraryPointer typeLibrary, TabAttribute**)
    {
        Enum<TabPosition> enm(typeLibrary, "TabPosition");
        enm.AddFormatter(TrimStart(11) + InsertSpaceBeforeCapitalLetters());
        enm.EnumValue("TabPositionAboveTab", TabPosition::AboveTab);
        enm.EnumValue("TabPositionInTab", TabPosition::InTab);
        enm.EnumValue("TabPositionBelowTab", TabPosition::BelowTab);

        Class<TabAttribute, Public<IAttribute>, std::shared_ptr<TabAttribute>> cls(typeLibrary, "TabAttribute");
        cls.Constructor<const std::string&>().AddSignature("tabName");
        cls.Constructor<TabPosition, const std::string&>().AddSignature("tabPosition", Arg("tabName") = std::string());
    }

    void ReflectAttributes(TypeLibraries::TypeLibraryPointer typeLibrary)
    {
        Reflect<BindToSelectionAttribute>(typeLibrary);
        Reflect<CaptionAttribute>(typeLibrary);
        Reflect<BindingAttribute>(typeLibrary);
        Reflect<TabAttribute>(typeLibrary);
        Reflect<GroupAttribute>(typeLibrary);
        Reflect<DefaultAttribute>(typeLibrary);
        Reflect<VisibleAttribute>(typeLibrary);
        Reflect<AllowCreateAndModifyAttribute>(typeLibrary);
        Reflect<AllowNullAttribute>(typeLibrary);
    }

}}}}


