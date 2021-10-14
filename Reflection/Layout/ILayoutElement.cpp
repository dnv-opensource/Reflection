//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutElement.h"
#include "AutoControl.h"
#include "CompareVisitor.h"
#include "StringizeVisitor.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {

    bool operator==(const ILayoutElement& lhs, const ILayoutElement& rhs)
    {
        return CompareVisitor(lhs, rhs);
    }

    bool operator!=(const ILayoutElement& lhs, const ILayoutElement& rhs)
    {
        return !operator==(lhs, rhs);
    }

    std::string ToString(const ILayoutElement& element)
    {
        if (!&element)
            return "nullptr";
        return StringizeVisitor(element);
    }

    LayoutElementPointer MakeElement(const std::string& name)
    {
        return std::make_shared<AutoControl>(name);
    }

}}}}

