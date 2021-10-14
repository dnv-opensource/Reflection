#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <memory>

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeLibraries {

    class ITypeLibrary;
    typedef std::shared_ptr<ITypeLibrary> TypeLibraryPointer;
}}}}
