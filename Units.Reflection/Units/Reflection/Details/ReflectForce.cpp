//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectForce.h"
#include "Units\Force.h"
#include "..\ReflectPhenomenon.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    using namespace DNVS::MoFa::Reflection::Classes;

    void ReflectForce( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        ReflectPhenomenon<ForcePhenomenon>(typeLibrary);
    }

}}}}