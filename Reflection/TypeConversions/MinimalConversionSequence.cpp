//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/MinimalConversionSequence.h"
#include <algorithm>

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    MinimalConversionSequence::MinimalConversionSequence(ArgumentConversionQuality quality, Types::DecoratedTypeInfo finalInfo)
        :   m_quality(quality)
        ,   m_finalInfo(finalInfo)
    {
    }

    MinimalConversionSequence::MinimalConversionSequence( const MinimalConversionSequence& other )
        :   m_quality(other.m_quality)
        ,   m_conversions(other.m_conversions)
        ,   m_finalInfo(other.m_finalInfo)
    {

    }

    MinimalConversionSequence::~MinimalConversionSequence()
    {
    }

    void MinimalConversionSequence::PushBack(const ConversionPointer& c)
    {
        m_conversions.push_back(c);
    }

    Variants::Variant MinimalConversionSequence::Convert(const Variants::Variant& input) const
    {
        Variants::Variant result = input;
        IntrusiveConvert(result);
        result.SetDecoratedTypeInfo(m_finalInfo);
        return result;
    }
    void MinimalConversionSequence::IntrusiveConvert(Variants::Variant& input) const
    {
        std::for_each(m_conversions.begin(),m_conversions.end(),[&input](const ConversionPointer& conversion)
        {
            conversion->IntrusiveConvert(input);
        });
    }

    bool MinimalConversionSequence::IsValid() const
    {
        return m_quality.IsValidConversion();
    }

    bool MinimalConversionSequence::Join(ConversionSequencePointer sequence )
    {
        std::shared_ptr<MinimalConversionSequence> other = std::dynamic_pointer_cast<MinimalConversionSequence>(sequence);
        if(!other) 
            return false;
        m_quality = m_quality + other->m_quality;
        std::for_each(other->m_conversions.begin(),other->m_conversions.end(),[this](const ConversionPointer& conversion)
        {
            m_conversions.push_back(conversion);
        });
        return true;
    }

}}}}
