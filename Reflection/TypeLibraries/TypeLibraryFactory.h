#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "TypeLibrary.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Reflection/TypeConversions/FullConversionGraph.h"

namespace DNVS { namespace MoFa { namespace Reflection { namespace TypeLibraries {
    class TypeLibraryFactory {
    public:
        template<typename ConversionGraphT>
        static TypeLibraryPointer CreateTypeLibrary(bool caseSensitive = true)
        {
            TypeConversions::ConversionGraphPointer conversionGraph(new ConversionGraphT);
            auto library = std::make_shared<TypeLibrary>(conversionGraph, caseSensitive);
            library->Initialize();
            return library;
        }

        static TypeLibraryPointer CreateDefaultTypeLibrary(bool caseSensitive = true)
        {
            return CreateTypeLibrary<TypeConversions::FullConversionGraph>(caseSensitive);
        }
    };
}}}}
