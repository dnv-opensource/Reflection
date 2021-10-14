//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ConversionGraph.h"
#include "Reflection/TypeConversions/ConversionSequence.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    struct ConversionGraphImpl
    {
        struct EdgeProperties 
        {
            EdgeProperties() {}
            ConversionPointer conversion;
        };

        typedef boost::adjacency_list
            <
                boost::setS, //EdgeList
                boost::vecS, //VertexList
                boost::directedS,
                boost::no_property, //VertexProperty
                boost::property 
                < 
                    boost::edge_weight_t, 
                    ArgumentConversionQuality,
                    EdgeProperties
                > //EdgeProperty
            > Graph;
        Graph::vertex_descriptor AddType(const Types::DecoratedTypeInfo& type)
        {
            auto ivertex = vertexMap.find(type);
            if(ivertex == vertexMap.end()) 
            {
                bool result;
                boost::tie(ivertex, result) = vertexMap.insert(std::make_pair(type, add_vertex(graph)));
            }
            return ivertex->second;
        }

        typedef boost::property_map<Graph, ConversionPointer EdgeProperties::*>::const_type ConstConversionMap;
        typedef boost::property_map<Graph, ConversionPointer EdgeProperties::*>::type ConversionMap;
        typedef boost::property_map<Graph, boost::edge_weight_t>::const_type ConstWeightMap;
        typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
        Graph graph;
        typedef std::map<Types::DecoratedTypeInfo, Graph::vertex_descriptor> VertexMap;
        VertexMap vertexMap;
    };
    ConversionGraph::ConversionGraph()
        :   m_invalidConversion(new ConversionSequence(ConversionType::InfiniteConversion))
        ,   m_noConversionNeeded(new ConversionSequence(ConversionType::NoConversion))
        ,   m_impl(new ConversionGraphImpl)
    {
    }

    ConversionGraph::~ConversionGraph()
    {

    }

    void ConversionGraph::AddConversion(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, ConversionType::Type conversionType, const ConversionPointer& conversion)
    {
        ConversionGraphImpl::Graph::edge_descriptor edgeId;
        bool result;
        boost::tie(edgeId, result) = add_edge(m_impl->AddType(from), m_impl->AddType(to), m_impl->graph);
        ConversionGraphImpl::ConversionMap conversions = get(&ConversionGraphImpl::EdgeProperties::conversion, m_impl->graph);
        ConversionGraphImpl::WeightMap weights = get(boost::edge_weight, m_impl->graph);
        //If this is a new conversion or a better conversion, use it.
        if(result || weights[edgeId] > conversionType) 
        {
            //Clear conversion cache.
            m_conversionSequences.clear();
            weights[edgeId] = conversionType;
            conversions[edgeId] = conversion;
        }
    }

    void ConversionGraph::AddWrapper(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to, const ConversionPointer& conversionFrom, const ConversionPointer& conversionTo)
    {

    }

    void ConversionGraph::AddAlternativeConverter(const Types::DecoratedTypeInfo& from, const AlternativeConverterPointer& alternativeConverter)
    {
        m_alternativeConverters.insert(std::make_pair(from, alternativeConverter));
    }

    ConversionSequencePointer ConversionGraph::GetConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const
    {
        ConversionSequencePointer sequence = GetConversionSequence(from.GetDecoratedTypeInfo(), to);
        if (!sequence || !sequence->IsValid())
        {
            ConversionSequencePointer alternativeSequence = GetAlternativeConversionSequence(from, to);
            if (alternativeSequence)
                return alternativeSequence;
        }
        return sequence;
    }

    TypeConversions::ConversionSequencePointer ConversionGraph::GetAlternativeConversionSequence(const Variants::Variant& from, const Types::DecoratedTypeInfo& to) const
    {
        for (const auto& converterPair : m_alternativeConverters)
        {
            if (converterPair.first == Types::TypeId<void>())
                continue;

            Variants::Variant input;
            ConversionSequencePointer sequencePre;
            sequencePre = GetConversionSequence(from.GetDecoratedTypeInfo(), converterPair.first);
            if (!sequencePre || !sequencePre->IsValid())
                continue;
            input = sequencePre->Convert(from);
            if (converterPair.second->CanConvert(input, to))
            {
                auto conversionSequence = std::make_shared<ConversionSequence>(converterPair.second->GetConversionType());
                conversionSequence->Add(converterPair.second->CreateConverter(to));
                if(sequencePre) conversionSequence->JoinBefore(sequencePre);
                if(conversionSequence->IsValid())
                    return conversionSequence;
            }
        }
        for (const auto& converterPair : m_alternativeConverters)
        {
            if (converterPair.first != Types::TypeId<void>())
                continue;

            Variants::Variant input;
            if (converterPair.second->CanConvert(from, to))
            {
                auto conversionSequence = std::make_shared<ConversionSequence>(converterPair.second->GetConversionType());
                conversionSequence->Add(converterPair.second->CreateConverter(to));
                if (conversionSequence->IsValid())
                    return conversionSequence;
            }
        }
        return nullptr;
    }

    Variants::Variant ConversionGraph::GetDynamicType( const Variants::Variant& from ) const
    {
        return from;
    }

    bool ConversionGraph::HasDynamicConversion(const std::type_info& type) const
    {
        return false;
    }

    ConversionSequencePointer ConversionGraph::GetConversionSequence(const Types::DecoratedTypeInfo& from, const Types::DecoratedTypeInfo& to) const
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        //MOFA_PROFILE_FUNCTION();
        if(from == to) 
            return m_noConversionNeeded;
        ConversionSequences::const_iterator iconversion = m_conversionSequences.find(std::make_pair(from, to));
        if(iconversion != m_conversionSequences.end()) 
            return iconversion->second;
        //If Variants::Variant is the general placeholder type that can represent any other type. 
        //Therefore, all types can be converted to Variants::Variant.
        if(to == Types::TypeId<Variants::Variant>()) 
        {
            ConversionSequencePointer conversionSequence(new ConversionSequence(ArgumentConversionQuality()));
            m_conversionSequences[std::make_pair(from, Types::TypeId<Variants::Variant>())] = conversionSequence;
            return conversionSequence;
        }

        auto ifrom = m_impl->vertexMap.find(from);
        auto ito = m_impl->vertexMap.find(to);
        if(ifrom == m_impl->vertexMap.end()) 
            return m_invalidConversion;
        if(ito == m_impl->vertexMap.end()) 
            return m_invalidConversion;

        std::vector<ConversionGraphImpl::Graph::vertex_descriptor> parent(num_vertices(m_impl->graph));
        std::vector<ArgumentConversionQuality> distances(num_vertices(m_impl->graph));
        ConversionGraphImpl::Graph::vertex_iterator vi, vi_end;
        for (boost::tie(vi, vi_end) = vertices(m_impl->graph); vi != vi_end; ++vi)
            parent[*vi] = *vi;

        boost::property_map<ConversionGraphImpl::Graph, boost::vertex_index_t>::type indexmap = get(boost::vertex_index, m_impl->graph);
        ConversionGraphImpl::ConversionMap conversions = get(&ConversionGraphImpl::EdgeProperties::conversion, m_impl->graph);
        ConversionGraphImpl::WeightMap weights = get(boost::edge_weight, m_impl->graph);

        dijkstra_shortest_paths(
            m_impl->graph, 
            ifrom->second, //Start vertex
            &parent[0],
            &distances[0],
            weights,
            indexmap, 
            std::less<ArgumentConversionQuality>(), 
            std::plus<ArgumentConversionQuality>(), 
            ConversionType::InfiniteConversion, ConversionType::NoConversion,
            boost::default_dijkstra_visitor()
            );

        ConversionGraphImpl::Graph::vertex_descriptor current = ito->second;
        ArgumentConversionQuality quality = distances[ito->second];
        if(!quality.IsValidConversion())
        {
            m_conversionSequences[std::make_pair(from, to)] = m_invalidConversion;
            return m_invalidConversion;
        }
        std::shared_ptr<ConversionSequence> conversionSequence(new ConversionSequence(distances[ito->second]));
        while(parent[current] != current) 
        {
            bool status;
            ConversionGraphImpl::Graph::edge_descriptor edgeId;
            boost::tie(edgeId, status) = edge(parent[current], current, m_impl->graph);
            if(!status) 
                return m_invalidConversion;
            conversionSequence->Add(conversions[edgeId]);
            current = parent[current];
        }
        if(current != ifrom->second) 
            return m_invalidConversion;
        else 
        {
            m_conversionSequences[std::make_pair(from, to)] = conversionSequence;
            return conversionSequence;
        }
    }

    Variants::Variant ConversionGraph::CreateLValue( const Variants::Variant& from ) const
    {
        throw std::runtime_error("Unable to create LValue for " + std::string(from.GetDecoratedTypeInfo().GetTypeInfo().name()));
    }

    Variants::Variant ConversionGraph::TryWrapValue(const Variants::Variant& from) const
    {
        return Variants::Variant();
    }

    Types::DecoratedTypeInfo ConversionGraph::TryUnwrapType(const Types::DecoratedTypeInfo& from) const
    {
        return from;
    }

    bool ConversionGraph::SupportsWrapping(const Types::DecoratedTypeInfo& from) const
    {
        return false;
    }

    bool ConversionGraph::SupportsUnwrapping(const Types::DecoratedTypeInfo& from) const
    {
        return false;
    }

    Variants::Variant ConversionGraph::TryUnwrapValue(const Variants::Variant& from) const
    {
        return Variants::Variant();
    }

}}}}
