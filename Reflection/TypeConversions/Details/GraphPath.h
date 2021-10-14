#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/TypeConversions/ArgumentConversionQuality.h"
#include "Reflection/TypeConversions/IConversionSequence.h"

#include <list>
#include <set>

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {
    class GraphEdge;
    class GraphVertex;

    class REFLECTION_IMPORT_EXPORT GraphPath 
    {
    public:
        GraphPath(const GraphVertex* startVertex);
        GraphPath(GraphPath&& other);
        GraphPath(const GraphPath& other);
        ///Returns false if the conversion involves two user conversions.
        bool IsValid() const;
        GraphPath Clone() const;
        const GraphVertex* GetHead() const;
        void SetHead(const GraphVertex* head);
        bool AddEdge(const GraphEdge* edge);
        const std::list<const GraphEdge*>& GetEdges() const;
        bool operator<(const GraphPath& other) const;
        ConversionSequencePointer CreateConversionSequence() const;
        /**Returns true if none of the conversion steps involved contains dynamic types.
        A dynamic type, such as DynamicQuantity, contains the unit not as a compile time type, but as a run time type.
        If we want to convert DynamicQuantity to Length (which is a compile time type), we cannot use type information alone to decide if the conversion is valid.
        We need to inspect the actual value we want to convert to see if the conversion is possible. 
        */
        bool IsDeterministic() const;
    private:
        bool m_deterministic;
        const GraphVertex* m_head;
        std::list<const GraphEdge*> m_edges;
        ArgumentConversionQuality m_quality;
    };
}}}}}