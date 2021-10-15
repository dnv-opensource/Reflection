//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "jsValue.Reflection.h"
#include "Reflection\Classes\Class.h"
#include "jsScript\jsConversions.h"

using namespace DNVS::MoFa::Reflection::TypeLibraries;
using namespace DNVS::MoFa::Reflection::Classes;

mofa::ref<jsValue> operator>(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator>(rhs);
}

mofa::ref<jsValue> operator<(mofa::ref<jsValue> lhs, jsValue* rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs)
        rhs = toJScript(0);
    return lhs->operator<(rhs);
}

mofa::ref<jsValue> operator>=(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator>=(rhs);
}

mofa::ref<jsValue> operator<=(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator<=(rhs);
}

mofa::ref<jsValue> operator!=(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (lhs.get() == rhs.get())
        return toJScript(false);
    if (lhs.get() && !rhs.get())
        return toJScript(true);
    if (!lhs.get() && rhs.get())
        return toJScript(true);
    return lhs->operator!=(rhs);
}

mofa::ref<jsValue> operator==(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (lhs.get() == rhs.get())
        return toJScript(true);
    if (lhs.get() && !rhs.get())
        return toJScript(false);
    if (!lhs.get() && rhs.get())
        return toJScript(false);
    return lhs->operator==(rhs);
}

mofa::ref<jsValue> operator/(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator/(rhs);
}

mofa::ref<jsValue> operator*(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator*(rhs);
}

mofa::ref<jsValue> operator+(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator+(rhs);
}

mofa::ref<jsValue> operator-(mofa::ref<jsValue> lhs, mofa::ref<jsValue> rhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    if (!rhs.get())
        rhs = toJScript(0);
    return lhs->operator-(rhs);
}

mofa::ref<jsValue> operator!(mofa::ref<jsValue> lhs)
{
    if(!lhs.get())
        return toJScript(true);
    return lhs->operator!();
}

mofa::ref<jsValue> operator-(mofa::ref<jsValue> lhs)
{
    if (!lhs.get())
        lhs = toJScript(0);
    return lhs->operator-();
}

void DoReflect(const TypeLibraryPointer& typeLibrary, jsValue**)
{
    Class<jsValue, mofa::ref<jsValue>> cls(typeLibrary, "jsValue");
    cls.Operator(This.Const.Storage / This.Const.Storage);
    cls.Operator(This.Const.Storage * This.Const.Storage);
    cls.Operator(This.Const.Storage > This.Const.Storage);
    cls.Operator(This.Const.Storage < *This);
    cls.Operator(This.Const.Storage >= This.Const.Storage);
    cls.Operator(This.Const.Storage <= This.Const.Storage);
    cls.Operator(This.Const.Storage != This.Const.Storage);
    cls.Operator(This.Const.Storage == This.Const.Storage);
    cls.Operator(This.Const.Storage + This.Const.Storage);
    cls.Operator(This.Const.Storage - This.Const.Storage);
    cls.Operator(!This.Const.Storage);
    cls.Operator(-This.Const.Storage);
}

void DoReflect(const TypeLibraryPointer& typeLibrary, jsScriptable**)
{
    Class<jsScriptable, Public<jsValue>, mofa::ref<jsScriptable>> cls(typeLibrary, "jsScriptable");
}

void DoReflect(const TypeLibraryPointer& typeLibrary, jsQuantity**)
{
    Class<jsQuantity, Public<jsScriptable>, mofa::ref<jsQuantity>> cls(typeLibrary, "jsQuantity");
}

void DoReflect(const TypeLibraryPointer& typeLibrary, jsUnitValue**)
{
    Class<jsUnitValue, Public<jsScriptable>, mofa::ref<jsUnitValue>> cls(typeLibrary, "jsUnitValue");
}

