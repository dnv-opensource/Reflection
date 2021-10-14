#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Object.h"
#include <typeindex>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    class IScriptingService {
    public:
        virtual ~IScriptingService() {}
        virtual Object Parse(const std::string& text,const std::type_index& hintType=typeid(void)) = 0;
        virtual TypeLibraries::TypeLibraryPointer GetTypeLibrary() const = 0;
        virtual std::string Stringize(const Object& object) const = 0;
    };
}}}}