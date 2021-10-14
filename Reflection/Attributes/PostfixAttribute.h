#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IAttribute.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /**
    The PostfixAttribute is used in reflection in order to indicate what unit a type has
    Example:
    Class<Length> cls(typeLibrary, "Length");
    cls.AddAttribute<Attributes::PostfixAttribute>([]()
    {
        return Runtime::GetInputUnitSystem().GetUnit(PhenomenonT()).GetUnitName();
    });

    This is the unit value that will be displayed after combo and edit boxes, such as m, mm, N, delC etc.
    */
    class PostfixAttribute : public IAttribute {
    public:
        typedef std::function<std::string()> PostfixCallback;
        PostfixAttribute(const PostfixCallback& callback) : m_postfixCallback(callback) {}
        std::string GetPostfix() const { return m_postfixCallback(); }
    private:
        PostfixCallback m_postfixCallback;
    };
}}}}