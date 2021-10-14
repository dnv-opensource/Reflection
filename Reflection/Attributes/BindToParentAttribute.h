#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS { namespace MoFa { namespace Reflection { namespace Attributes {
    /*
    Use BindToParentAttribute if you have a tree of object, for example:
    LinearVaryingCurveOffset(
        ConstantCurveOffset(Vector3d(0, 0, 1), true),
        ConstantCurveOffset(Vector3d(0, 0, 2), true),
        true
    );
    We want the ConstantCurveOffsets to inherit the second argument (UseLocalSystem) from the parent,
    The LinearVaryingCurveOffset.
    The way to do this is to add the BindToParentAttribute to the ConstantCurveOffsets UseLocalSystem:
    cls.SetGet("UseLocalSystem", &ConstantCurveOffset::UseLocalSystem, &ConstantCurveOffset::UseLocalSystem)
       .AddAttribute<BindToParentAttribute>();
    When building up the dialog for this, the child member will be bound to the value of the parent member.
    */
    class BindToParentAttribute {};
}}}}
