//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectDampingPerUnitLength.h"
#include "Units\DampingPerUnitLength.h"
#include "..\ReflectPhenomenon.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    using namespace DNVS::MoFa::Reflection::Classes;

    void ReflectDampingPerUnitLength( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        ReflectPhenomenon<DampingPerUnitLengthPhenomenon>(typeLibrary);
    }

}}}}