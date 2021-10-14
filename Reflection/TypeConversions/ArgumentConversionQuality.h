#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Reflection/Config.h"

#include "Reflection/TypeConversions/ConversionType.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    class REFLECTION_IMPORT_EXPORT ArgumentConversionQuality
    {
    public:
        ArgumentConversionQuality();
        ///Creates a new conversion quality based on the conversion type of a single conversion.
        ArgumentConversionQuality(ConversionType::Type conversionType);
        ArgumentConversionQuality(const ArgumentConversionQuality& other);
        ArgumentConversionQuality& operator=(const ArgumentConversionQuality& other);
        ///Adds two conversion qualities.
        ArgumentConversionQuality operator+(const ArgumentConversionQuality& rhs) const;
        ///Compares two conversion qualities. Returns true if this is a better conversion than rhs.
        bool operator<(const ArgumentConversionQuality& rhs) const;
        ///Compares two conversion qualities. Returns true if they have the same quality.
        bool operator==(const ArgumentConversionQuality& rhs) const;
        bool operator!=(const ArgumentConversionQuality& rhs) const {return !operator==(rhs);}
        bool operator>(const ArgumentConversionQuality& rhs) const {return rhs<*this;}
        bool operator>=(const ArgumentConversionQuality& rhs) const {return !(*this<rhs);}
        bool operator<=(const ArgumentConversionQuality& rhs) const {return !(rhs<*this);}
        ///Returns true if this conversion is valid.
        bool IsValidConversion() const;
        bool IsUpcast() const;
    private:
        ///Constructor only used by the operator+
        ArgumentConversionQuality(size_t trivialConversions, size_t promotionConversions, size_t standardConversions, size_t userConversions, bool isUpcast);
        size_t m_trivialConversions;
        size_t m_promotionConversions;
        size_t m_standardConversions;
        size_t m_userConversions;
        bool m_isUpcast;
    };
}}}}