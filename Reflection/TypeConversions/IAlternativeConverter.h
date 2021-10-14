#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IConversion.h"

namespace DNVS {namespace MoFa { namespace Reflection { namespace TypeConversions {
    /*
    This interface can be used if you have a class that needs runtime inspection to see if it can be converted to the returnType.
    Typical use case is to allow conversion from jsArray to std::list<int>.
    */
    class IAlternativeConverter {
    public:
        //Checks whether variant can be converted to returnType.
        virtual bool CanConvert(const Variants::Variant& variant, const Types::DecoratedTypeInfo& returnType) const = 0;
        //Create a converter that can convert objects of type jsArray to the container specified by returnType.
        virtual TypeConversions::ConversionPointer CreateConverter(const Types::DecoratedTypeInfo& returnType) const = 0;
        //Returns the conversion quality of the conversion created by this class.
        virtual ConversionType::Type GetConversionType() const = 0;
    };

    typedef std::shared_ptr<IAlternativeConverter> AlternativeConverterPointer;
}}}}