#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "boost/optional/optional.hpp"
#include "Reflection/AutoReflect.h"
#include "Reflection/Classes/Class.h"
#include "Reflection/Attributes/OptionalAttribute.h"
#include <memory>
#define REFLECTOR_LIST_INCLUDED

namespace DNVS {namespace MoFa {namespace Reflection {
    template<typename T>
    struct AutoReflectImplemented<boost::optional<T>, void>
    {
        static const bool value = true;
    };
}}}


namespace std {
    template<typename T>
    boost::optional<T>* get_pointer(std::unique_ptr<boost::optional<T>> const& p) { return p.get(); }
    template<typename T>
    boost::optional<T>* get_pointer(std::shared_ptr<boost::optional<T>> const& p) { return p.get(); }
}

namespace boost
{
    template<typename T>
    void DoReflect(const DNVS::MoFa::Reflection::TypeLibraries::TypeLibraryPointer& typeLibrary, boost::optional<T>**)
    {
        using namespace DNVS::MoFa::Reflection::Classes;
        DNVS::MoFa::Reflection::AutoReflect<T>::Reflect(typeLibrary);
        Class<boost::optional<T>, DNVS::MoFa::Reflection::IgnoreAutoReflector, std::unique_ptr<boost::optional<T>>> cls(typeLibrary, "__optional__");
        cls.Constructor<const T&>();
        cls.Constructor();
        cls.AddAttribute<OptionalAttribute>(DNVS::MoFa::Reflection::Types::TypeId<T>());
    };
}