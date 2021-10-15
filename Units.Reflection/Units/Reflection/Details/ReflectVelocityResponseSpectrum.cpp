//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectVelocityResponseSpectrum.h"
#include "Units\VelocityResponseSpectrum.h"
#include "..\ReflectPhenomenon.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    using namespace DNVS::MoFa::Reflection::Classes;

    void ReflectVelocityResponseSpectrum( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        ReflectPhenomenon<VelocityResponseSpectrumPhenomenon>(typeLibrary);
    }

}}}}