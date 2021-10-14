#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Variants/VariantService.h"
#include "DefaultOptions.h"
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    Use this attribute on members representing a polymorphic base class.
    For example, CurveOffsetPointer (which represents ICurveOffset).
    DefaultTypeAttribute should specify the default type we want to present in a dialog.
    If, for example, you specify
    cls.AddAttribute<DefaultTypeAttribute>(typeid(ConstantCurveOffset));
    The member will be populated with a ConstantCurveOffset, and the corresponding dialog will be
    made available for the user.
    */
    class DefaultTypeAttribute : public IAttribute {
    public:
        DefaultTypeAttribute(const std::type_info& type) 
            : m_type(type) 
            , m_storeDefault(false)
        {}
        DefaultTypeAttribute(const std::type_info& type, const Impl::StoreDefault& storeDefault)
            : m_type(type)
            , m_storeDefault(true)
        {}
        DefaultTypeAttribute(const Impl::StoreDefault& storeDefault, const std::type_info& type)
            : DefaultTypeAttribute(type, storeDefault)
        {}
        DefaultTypeAttribute(const Impl::StoreDefault& storeDefault)
            : DefaultTypeAttribute(typeid(void), storeDefault)
        {}
        const std::type_info& GetDefaultType() const
        {
            return m_type;
        }
        bool HasDefaultType() const
        {
            return m_type != typeid(void);
        }
        bool StoreDefault() const { return m_storeDefault; }
    private:
        const std::type_info& m_type;
        bool m_storeDefault;
    };
}}}}
#pragma once
