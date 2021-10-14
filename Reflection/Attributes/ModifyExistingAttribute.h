#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Set this attribute when you want to disable read only fields during modification.
    */
    namespace Impl {
        class DisableSelectionOfModified {};
    }
    constexpr Impl::DisableSelectionOfModified DisableSelectionOfModified;

    class ModifyExistingAttribute : public IAttribute
    {
    public:
        ModifyExistingAttribute()
            : m_disableSelectionOfModified(false)
        {}
        ModifyExistingAttribute(Impl::DisableSelectionOfModified)
            : m_disableSelectionOfModified(true)
        {}
        bool GetDisableSelectionOfModified() const { return m_disableSelectionOfModified; }
    private:
        bool m_disableSelectionOfModified;
    };
}}}}