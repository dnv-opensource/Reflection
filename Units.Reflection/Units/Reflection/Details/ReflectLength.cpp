//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ReflectLength.h"
#include "Units\Length.h"
#include "..\ReflectPhenomenon.h"
#include "Units\LengthX.h"
#include "Units\LengthY.h"
#include "Units\LengthZ.h"
namespace DNVS {namespace MoFa {namespace Units { namespace Reflection {
    using namespace DNVS::MoFa::Reflection::Classes;
    using namespace DNVS::MoFa::Reflection;
    template<typename T>
    void RegisterDirectionalLength(DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary, const std::string& name)
    {
        Class<T, Public<Length>, IgnoreAutoReflector> cls(typeLibrary, name);
        cls.Constructor<const Quantity<LengthDimension>&>();
        cls.Constructor([](double value) {return T(DNVS::MoFa::Units::Runtime::GetInputUnitSystem().ConvertFromUnitSystem<Length>(value)); });
        cls.Constructor<Length>(Alias);
        RegisterToStringFunction(cls);
    }
    void ReflectLength( DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer typeLibrary )
    {
        ReflectPhenomenon<LengthPhenomenon>(typeLibrary);
        RegisterDirectionalLength<LengthX>(typeLibrary, "LengthX");
        RegisterDirectionalLength<LengthY>(typeLibrary, "LengthY");
        RegisterDirectionalLength<LengthZ>(typeLibrary, "LengthZ");
    }

}}}}