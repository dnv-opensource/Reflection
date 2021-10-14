#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/TypeConversions/ArgumentConversionQuality.h"
#include "Reflection/Variants/Variant.h"
#include <memory>


namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {
    class IConversionSequence;
    typedef std::shared_ptr<IConversionSequence> ConversionSequencePointer;

    class REFLECTION_IMPORT_EXPORT IConversionSequence
    {
    public:
        virtual ~IConversionSequence() {}
        virtual bool IsValid() const = 0;
        virtual Variants::Variant Convert(const Variants::Variant& input) const = 0;
        virtual void IntrusiveConvert(Variants::Variant& input) const = 0;
        virtual const ArgumentConversionQuality& Quality() const = 0;
        virtual bool Join(ConversionSequencePointer sequence) = 0;
        virtual bool JoinBefore(ConversionSequencePointer sequence) = 0;
    };

}}}}