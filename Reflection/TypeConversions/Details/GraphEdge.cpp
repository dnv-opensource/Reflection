//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GraphEdge.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {

    void GraphEdge::SetConversionType( ConversionType::Type conversionType )
    {
        m_conversionType = conversionType;
    }

    ConversionType::Type GraphEdge::GetConversionType() const
    {
        return m_conversionType;
    }

    void GraphEdge::SetConversion( const ConversionPointer& conversion )
    {
        m_conversion = conversion;
    }

    GraphEdge::GraphEdge(const GraphVertex* vertex ) 
        :   m_conversionType(ConversionType::InfiniteConversion)
        ,   m_endVertex(vertex)
    {
    }

    ConversionPointer GraphEdge::GetConversion() const
    {
        return m_conversion;
    }

    bool GraphEdge::operator<( const GraphEdge& other ) const
    {
        return m_endVertex<other.m_endVertex;
    }

    const GraphVertex* GraphEdge::GetEndVertex() const
    {
        return m_endVertex;
    }

}}}}}