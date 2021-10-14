//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/TypeConversions/ArgumentConversionQuality.h"
#include "limits"

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    ArgumentConversionQuality::ArgumentConversionQuality()
        :   m_trivialConversions(0)
        ,   m_promotionConversions(0)
        ,   m_standardConversions(0)
        ,   m_userConversions(0)
        ,   m_isUpcast(false)
    {}

    ArgumentConversionQuality::ArgumentConversionQuality(ConversionType::Type conversionType)
        :   m_trivialConversions(conversionType == ConversionType::TrivialConversion?1:0)
        ,   m_promotionConversions(conversionType == ConversionType::PromotionConversion?1:0)
        ,   m_standardConversions(conversionType == ConversionType::StandardConversion?1:0)
        ,   m_userConversions(conversionType == ConversionType::UserConversion?1:0)
        ,   m_isUpcast(conversionType == ConversionType::UpcastConversion)
    {
        if(m_isUpcast) 
            m_standardConversions = 1;
        if(conversionType == ConversionType::InfiniteConversion) 
        {
            m_trivialConversions = m_promotionConversions = m_standardConversions = m_userConversions = (std::numeric_limits<size_t>::max)();
        }
        //Choose the generic conversion last.
        if (conversionType == ConversionType::ReflectionConversion)
        {
            m_userConversions = 1;
            m_trivialConversions = m_promotionConversions = m_standardConversions = (std::numeric_limits<size_t>::max)();
        }
    }

    ArgumentConversionQuality::ArgumentConversionQuality(size_t trivialConversions, size_t promotionConversions, size_t standardConversions, size_t userConversions, bool isUpcast)
        : m_trivialConversions(trivialConversions)
        , m_promotionConversions(promotionConversions)
        , m_standardConversions(standardConversions)
        , m_userConversions(userConversions)
        , m_isUpcast(isUpcast)
    {
    }

    ArgumentConversionQuality::ArgumentConversionQuality(const ArgumentConversionQuality& other)
        : m_trivialConversions(other.m_trivialConversions)
        , m_promotionConversions(other.m_promotionConversions)
        , m_standardConversions(other.m_standardConversions)
        , m_userConversions(other.m_userConversions)
        , m_isUpcast(other.m_isUpcast)
    {
    }

    ArgumentConversionQuality& ArgumentConversionQuality::operator=(const ArgumentConversionQuality& other)
    {
        if(this != &other) 
        {
            m_trivialConversions = other.m_trivialConversions;
            m_promotionConversions = other.m_promotionConversions;
            m_standardConversions = other.m_standardConversions;
            m_userConversions = other.m_userConversions;
            m_isUpcast = other.m_isUpcast;
        }
        return *this;
    }

    ArgumentConversionQuality ArgumentConversionQuality::operator+(const ArgumentConversionQuality& rhs) const
    {
        if(!IsValidConversion()) 
            return *this;
        if(!rhs.IsValidConversion()) 
            return rhs;
        //Disallow user conversions followed by casts. This to prevent stupid conversions from int through SomeRandomClass to jsValue.
        if(m_userConversions>0 && rhs.m_isUpcast) 
            return ArgumentConversionQuality(ConversionType::InfiniteConversion);
        return ArgumentConversionQuality(
            m_trivialConversions + rhs.m_trivialConversions,
            m_promotionConversions + rhs.m_promotionConversions,
            m_standardConversions + rhs.m_standardConversions,
            m_userConversions + rhs.m_userConversions,
            m_isUpcast || rhs.m_isUpcast
            );
    }

    bool ArgumentConversionQuality::operator<(const ArgumentConversionQuality& rhs) const
    {
        if(m_userConversions != rhs.m_userConversions) 
            return m_userConversions<rhs.m_userConversions;
        if(m_standardConversions != rhs.m_standardConversions) 
            return m_standardConversions<rhs.m_standardConversions;
        if(m_promotionConversions != rhs.m_promotionConversions) 
            return m_promotionConversions<rhs.m_promotionConversions;
        return m_trivialConversions<rhs.m_trivialConversions;
    }
    bool ArgumentConversionQuality::operator==(const ArgumentConversionQuality& rhs) const
    {
        if(m_userConversions != rhs.m_userConversions) 
            return false;
        if(m_standardConversions != rhs.m_standardConversions) 
            return false;
        if(m_promotionConversions != rhs.m_promotionConversions) 
            return false;
        return m_trivialConversions == rhs.m_trivialConversions;
    }
    bool ArgumentConversionQuality::IsValidConversion() const
    {
        if(m_userConversions>1) 
            return false;
        else 
            return true;
    }


    bool ArgumentConversionQuality::IsUpcast() const
    {
        return m_isUpcast;
    }

}}}}