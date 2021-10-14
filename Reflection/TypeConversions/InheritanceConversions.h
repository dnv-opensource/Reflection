#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DowncastConversion.h"
#include "UpcastConversion.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace TypeConversions {
    template<typename Base,typename Derived>
    void AddInheritanceConversions(ConversionGraphPointer conversionGraph) {
        AddDowncastConversion<Base,Derived>(conversionGraph);
        AddUpcastConversion<Derived,Base>(conversionGraph);
    }
}}}}