//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ManualConversionGraph.h"
#include "Reflection/TypeConversions/ArgumentConversionQuality.h"
#include "Reflection/TypeConversions/ConversionSequence.h"
#include "Reflection/TypeConversions/Details/GraphPath.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    
    void ManualConversionGraph::AddConversion( const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion )
    {
        m_conversionSequences.clear();
        Details::GraphVertex* start = LookupVertexCreateIfMissing(from);
        Details::GraphVertex* end = LookupVertexCreateIfMissing(to);
        start->AddConversion(end, conversionType, conversion);
    }

    template<typename FunctorT>
    ConversionSequencePointer TypeConversions::ManualConversionGraph::GetConversionSequenceImpl( const Types::DecoratedTypeInfo& from,const Types::DecoratedTypeInfo& to,FunctorT& pathAdjuster) const
    {
        if(from==to)
            return ConversionSequencePointer(new ConversionSequence(ConversionType::NoConversion));
        auto icache = m_conversionSequences.find(std::make_pair(from,to));
        if(icache!=m_conversionSequences.end())
            return icache->second;

        const Details::GraphVertex* start = LookupVertex(from);
        const Details::GraphVertex* end = LookupVertex(to);
        if(start == nullptr || end == nullptr)
        {
            return ConversionSequencePointer(new ConversionSequence(ConversionType::InfiniteConversion));
        }
        else {
            std::multiset<Details::GraphPath> paths;
            paths.insert(Details::GraphPath(start));
            std::set<const Details::GraphVertex*> usedVertices;
            usedVertices.insert(start);
            while(!paths.empty() && paths.begin()->GetHead()!=end)
            {
                Details::GraphPath path=*paths.begin();
                paths.erase(paths.begin());
                for(auto it=path.GetHead()->GetOutEdges().begin();it!=path.GetHead()->GetOutEdges().end();++it) {
                    const Details::GraphEdge& edge=*it;
                    if(!usedVertices.insert(edge.GetEndVertex()).second) 
                        continue;
                    Details::GraphPath newPath(path);
                    if(newPath.AddEdge(&edge)) 
                    {
                        pathAdjuster(newPath,edge.GetConversionType());
                        paths.insert(std::move(newPath));
                    }
                }
            }
            if(paths.empty())
                return ConversionSequencePointer(new ConversionSequence(ConversionType::InfiniteConversion));
            else
            {
                const Details::GraphPath& path=*paths.begin();
                ConversionSequencePointer conversionSequence = path.CreateConversionSequence();
                if(path.IsDeterministic()) 
                    m_conversionSequences[std::make_pair(from,to)] = conversionSequence;
                return conversionSequence;
            }
        }
    }

    TypeConversions::ConversionSequencePointer ManualConversionGraph::GetConversionSequence( const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to ) const
    {
        return GetConversionSequenceImpl(from, to, [](Details::GraphPath& path,ConversionType::Type type) {});
    }

    TypeConversions::ConversionSequencePointer ManualConversionGraph::GetConversionSequence( const Variants::Variant& from, const Types::DecoratedTypeInfo& to ) const
    {
        return GetConversionSequenceImpl(from.GetDecoratedTypeInfo(), to, [&from, this](Details::GraphPath& path,ConversionType::Type type) 
        {
            this->AdjustHeadIfNecessary(path,type,from);
        });
    }

    void ManualConversionGraph::AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter)
    {

    }

    Variants::Variant ManualConversionGraph::GetDynamicType(const Variants::Variant& from) const
    {
        const Details::GraphVertex* vertex = LookupVertex(from.GetDecoratedTypeInfo());
        if(!vertex) return from;
        for(auto it=vertex->GetOutEdges().begin();it!=vertex->GetOutEdges().end();++it)
        {
            const Details::GraphEdge& edge=*it;
            if(edge.GetConversionType()==ConversionType::DynamicTypeConversion)
            {
                Variants::Variant converted = edge.GetConversion()->Convert(from);                
                return GetDynamicType(converted);
            }
        }
        return from;
    }

    bool ManualConversionGraph::HasDynamicConversion(const std::type_info& type) const
    {
        return false;
    }

    Variants::Variant ManualConversionGraph::CreateLValue(const Variants::Variant& from) const
    {
        const Details::GraphVertex* vertex = LookupVertex(from.GetDecoratedTypeInfo());
        if(vertex)
        {
            for(auto it=vertex->GetOutEdges().begin();it!=vertex->GetOutEdges().end();++it)
            {
                const Details::GraphEdge& edge=*it;
                if(edge.GetConversionType()==ConversionType::LValueConversion)
                {
                    return edge.GetConversion()->Convert(from);
                }
            }
        }
        throw std::runtime_error("Unable to convert " + std::string(from.GetDecoratedTypeInfo().GetTypeInfo().name()) + " into a variable");
    }

    Details::GraphVertex* ManualConversionGraph::LookupVertexCreateIfMissing( const Types::DecoratedTypeInfo& from )
    {
        auto it = m_vertices.find(from);
        if(it==m_vertices.end())
            it=m_vertices.insert(std::make_pair(from,Details::GraphVertex(from))).first;
        return &it->second;
    }

    const Details::GraphVertex* ManualConversionGraph::LookupVertex( const Types::DecoratedTypeInfo& from ) const
    {
        auto it = m_vertices.find(from);
        if(it==m_vertices.end())
            return nullptr;
        else
            return &it->second;
    }

    void ManualConversionGraph::AdjustHeadIfNecessary( Details::GraphPath& path, ConversionType::Type conversionType, const Variants::Variant& from ) const
    {
        if(conversionType == ConversionType::FallbackConversion || conversionType == ConversionType::DynamicTypeConversion)
        {
            Variants::Variant convertedValue = path.CreateConversionSequence()->Convert(from);
            const Details::GraphVertex* newVertex = LookupVertex(convertedValue.GetDecoratedTypeInfo());
            if(newVertex) 
                path.SetHead(newVertex);
        }
    }

    Variants::Variant ManualConversionGraph::TryWrapValue(const Variants::Variant& from) const
    {
        return Variants::Variant();
    }

    bool ManualConversionGraph::SupportsWrapping(const Types::DecoratedTypeInfo& from) const
    {
        return false;
    }

    Variants::Variant ManualConversionGraph::TryUnwrapValue(const Variants::Variant& from) const
    {
        return Variants::Variant();
    }

    Types::DecoratedTypeInfo ManualConversionGraph::TryUnwrapType(const Types::DecoratedTypeInfo& from) const
    {
        return from;
    }

    void ManualConversionGraph::AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo)
    {
    }

    bool ManualConversionGraph::SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const
    {
        throw std::logic_error("The method or operation is not implemented.");
    }

}}}}