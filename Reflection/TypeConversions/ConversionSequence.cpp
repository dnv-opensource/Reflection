//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ConversionSequence.h"
#include <algorithm>

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    ConversionSequence::ConversionSequence(ArgumentConversionQuality quality)
        :   m_quality(quality)
    {
    }

    ConversionSequence::ConversionSequence( const ConversionSequence& other )
        :   m_quality(other.m_quality)
        ,   m_conversions(other.m_conversions)
    {

    }

    ConversionSequence::~ConversionSequence()
    {
    }

    void ConversionSequence::Add(const ConversionPointer& c)
    {
        m_conversions.push_front(c);
    }

    Variants::Variant ConversionSequence::Convert(const Variants::Variant& input) const
    {
        Variants::Variant result = input;
        for(const auto& conversion : m_conversions)
        {
            result = conversion->Convert(result);
        }
        return result;
    }
    void ConversionSequence::IntrusiveConvert(Variants::Variant& input) const
    {
        for(const auto& conversion : m_conversions)
        {
            conversion->IntrusiveConvert(input);
        }
    }

    bool ConversionSequence::IsValid() const
    {
        return m_quality.IsValidConversion();
    }

    bool ConversionSequence::Join(ConversionSequencePointer sequence )
    {
        std::shared_ptr<ConversionSequence> other = std::dynamic_pointer_cast<ConversionSequence>(sequence);
        if(!other) 
            return false;
        m_quality = m_quality + other->m_quality;
        std::for_each(other->m_conversions.begin(),other->m_conversions.end(),[this](const ConversionPointer& conversion)
        {
            m_conversions.push_back(conversion);
        });
        return true;
    }

    bool ConversionSequence::JoinBefore(ConversionSequencePointer sequence)
    {
        std::shared_ptr<ConversionSequence> other = std::dynamic_pointer_cast<ConversionSequence>(sequence);
        if (!other)
            return false;
        m_quality = m_quality + other->m_quality;
        std::for_each(other->m_conversions.rbegin(), other->m_conversions.rend(), [this](const ConversionPointer& conversion)
        {
            m_conversions.push_front(conversion);
        });
        return true;
    }

}}}}
