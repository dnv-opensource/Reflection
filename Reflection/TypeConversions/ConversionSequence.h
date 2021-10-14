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

    class REFLECTION_IMPORT_EXPORT ConversionSequence : public IConversionSequence
    {
    public:
        ConversionSequence(ArgumentConversionQuality quality);
        ConversionSequence(const ConversionSequence& other);
        virtual ~ConversionSequence();
        void Add(const ConversionPointer& c);
        virtual bool IsValid() const;
        virtual Variants::Variant Convert(const Variants::Variant& input) const;
        virtual void IntrusiveConvert(Variants::Variant& input) const;
        virtual const ArgumentConversionQuality& Quality() const {return m_quality; } 
        virtual bool Join(ConversionSequencePointer sequence);
        virtual bool JoinBefore(ConversionSequencePointer sequence);
    private:
        std::list<ConversionPointer> m_conversions;
        ArgumentConversionQuality m_quality;
    };

}}}}