//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GraphVertex.h"
#include "Reflection/TypeConversions/ArgumentConversionQuality.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {

    void GraphVertex::AddConversion( GraphVertex* vertex, ConversionType::Type conversionType, const ConversionPointer& conversion )
    {
        GraphEdge& edge = const_cast<GraphEdge&>(*m_outEdges.insert(vertex).first);
        if(IsConversionBetter(conversionType, edge.GetConversionType()))
        {
            edge.SetConversionType(conversionType);
            edge.SetConversion(conversion);
        }
    }

    bool GraphVertex::IsConversionBetter( ConversionType::Type newConversion, ConversionType::Type base ) const
    {
        return ArgumentConversionQuality(newConversion)<ArgumentConversionQuality(base);
    }

    GraphVertex::GraphVertex( Types::DecoratedTypeInfo info ) : m_info(info)
    {

    }

    const Types::DecoratedTypeInfo& GraphVertex::GetType() const
    {
        return m_info;
    }

}}}}}