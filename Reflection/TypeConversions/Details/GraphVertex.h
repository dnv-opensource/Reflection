#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/Types/DecoratedTypeInfo.h"

#include "GraphEdge.h"
#include "Reflection/TypeConversions/ConversionType.h"
#include "Reflection/TypeConversions/IConversion.h"

#include <set>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {
    class REFLECTION_IMPORT_EXPORT GraphVertex 
    {
    public:
        GraphVertex(Types::DecoratedTypeInfo info);
        const Types::DecoratedTypeInfo& GetType() const;
        void AddConversion(GraphVertex* vertex, ConversionType::Type conversionType, const ConversionPointer& conversion);
        bool IsConversionBetter( ConversionType::Type newConversion, ConversionType::Type base ) const;
        const std::set<GraphEdge>& GetOutEdges() const {return m_outEdges; }
    private:
        Types::DecoratedTypeInfo m_info;
        std::set<GraphEdge> m_outEdges;
    };
}}}}}