#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/TypeConversions/IConversion.h"
#include "Reflection/TypeConversions/IConversionSequence.h"
#include <list>

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    class MinimalConversionSequence : public IConversionSequence
    {
    public:
        MinimalConversionSequence(ArgumentConversionQuality quality, Types::DecoratedTypeInfo finalInfo);
        MinimalConversionSequence(const MinimalConversionSequence& other);
        virtual ~MinimalConversionSequence();
        void PushBack(const ConversionPointer& c);
        virtual bool IsValid() const;
        virtual Variants::Variant Convert(const Variants::Variant& input) const;
        virtual void IntrusiveConvert(Variants::Variant& input) const;
        virtual const ArgumentConversionQuality& Quality() const {return m_quality; } 
        virtual bool Join(ConversionSequencePointer sequence);
    private:
        std::list<ConversionPointer> m_conversions;
        Types::DecoratedTypeInfo m_finalInfo;
        ArgumentConversionQuality m_quality;
    };

}}}}