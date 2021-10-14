#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <string>
#include "Services/Module.h"
#include "Reflection/Config.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Objects { class Object; }}}}

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The IconAttribute is used to add an icon to a type or constructor.
    Example:
    Class<scStraightBeam,...> cls(typeLibrary, "StraightBeam");
    cls.AddAttribute<IconAttribute>(IDB_StraightBeamIcon);
    It will also be used when you select which constructor to use in nested dialogs.

    The icon will be displayed next to the item when shown in a combo box.
    */
    class IconAttribute : public IAttribute {
    public:
        IconAttribute(int iconId, Services::Module module = Services::Module())
            : m_iconId(iconId)
            , m_module(module)
        {}
        int GetIconId() const { return m_iconId; }
        const Services::Module& GetModule() const { return m_module; }
    private:
        int m_iconId;
        Services::Module m_module;
    };

    const REFLECTION_IMPORT_EXPORT IconAttribute* TryGetIconAttributeFromObject(const Objects::Object& object);
}}}}
