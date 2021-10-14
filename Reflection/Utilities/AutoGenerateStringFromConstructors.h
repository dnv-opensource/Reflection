#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {
    /*
    Will use constructors found for object to auto-generate a string representing the object.
    If you have the following constructors:
    Plane3d(Point, Vector3d)
    ZPlane3d(double z);
    YPlane3d(double y);
    XPlane3d(double x);
    Plane3d operator-() const;
    The plane:
    Plane3d(Point(0,1,4), Vector3d(0,0,-1))
    will be formatted as -ZPlane3d(4);
    see ConstructorSelector for more details.
    */
    std::string REFLECTION_IMPORT_EXPORT AutoGenerateStringFromConstructors(Objects::Object object, const DNVS::MoFa::Formatting::FormattingService& formattingService);
}}}}
