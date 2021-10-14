//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "GraphPath.h"
#include "Reflection/TypeConversions/ConversionSequence.h"
#include "GraphEdge.h"
#include "GraphVertex.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {

    ConversionSequencePointer GraphPath::CreateConversionSequence() const
    {
        ConversionSequence* sequence = new ConversionSequence(m_quality);
        for(auto it = m_edges.rbegin(); it != m_edges.rend(); ++it)
        {
            const GraphEdge* edge = *it;
            sequence->Add(edge->GetConversion());
        }
        return ConversionSequencePointer(sequence);
    }

    bool GraphPath::operator<( const GraphPath& other ) const
    {
        return m_quality<other.m_quality;
    }

    const std::list<const GraphEdge*>& GraphPath::GetEdges() const
    {
        return m_edges;
    }

    bool GraphPath::AddEdge( const GraphEdge* edge )
    {
        m_head = edge->GetEndVertex();
        m_edges.push_back(edge);
        m_quality = m_quality + ArgumentConversionQuality(edge->GetConversionType());
        return IsValid();
    }

    const GraphVertex* GraphPath::GetHead() const
    {
        return m_head;
    }

    TypeConversions::Details::GraphPath GraphPath::Clone() const
    {
        return GraphPath(*this);
    }

    bool GraphPath::IsValid() const
    {
        return m_quality.IsValidConversion();
    }

    GraphPath::GraphPath( const GraphPath& other ) :   m_head(other.m_head)
        ,   m_edges(other.m_edges)
        ,   m_quality(other.m_quality)
        ,   m_deterministic(other.m_deterministic)
    {

    }

    GraphPath::GraphPath( GraphPath&& other ) :   m_head(other.m_head)
        ,   m_edges(std::move(other.m_edges))
        ,   m_quality(other.m_quality)
        ,   m_deterministic(other.m_deterministic)
    {

    }

    GraphPath::GraphPath( const GraphVertex* startVertex )
        :   m_head(startVertex)
        ,   m_deterministic(true)
    {
    }

    void GraphPath::SetHead( const GraphVertex* head )
    {
        m_head = head;
        m_deterministic = false;
    }

    bool GraphPath::IsDeterministic() const
    {
        return m_deterministic;
    }

}}}}}