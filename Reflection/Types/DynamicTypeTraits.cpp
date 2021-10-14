//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DynamicTypeTraits.h"
#include "Reflection/TypeConversions/IConversionGraph.h"
#include "Reflection/TypeConversions/IConversionSequence.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Types {

    bool IsBaseOf(const std::shared_ptr<TypeConversions::IConversionGraph>& conversionGraph, const DecoratedTypeInfo& base, const DecoratedTypeInfo& derived)
    {
        if (IsSameAs(base, derived))
            return true;
        std::shared_ptr<TypeConversions::IConversionSequence> sequence = conversionGraph->GetConversionSequence(derived, base);
        if (!sequence)
            return false;
        return sequence->Quality().IsUpcast();
    }

}}}}

