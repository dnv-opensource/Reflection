#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/TypeConversions/ConversionType.h"
#include "Reflection/TypeConversions/IConversion.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions { namespace Details {
    class GraphVertex;

    class REFLECTION_IMPORT_EXPORT GraphEdge 
    {
    public:
        GraphEdge(const GraphVertex* endVertex);
        ConversionType::Type GetConversionType() const;
        void SetConversionType( ConversionType::Type conversionType );
        void SetConversion( const ConversionPointer& conversion );
        ConversionPointer GetConversion() const;
        bool operator<(const GraphEdge& other) const;
        const GraphVertex* GetEndVertex() const;
    private:
        const GraphVertex* m_endVertex;
        ConversionPointer m_conversion;
        ConversionType::Type m_conversionType;
    };
}}}}}