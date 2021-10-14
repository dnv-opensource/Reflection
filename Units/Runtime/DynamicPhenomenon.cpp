//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DynamicPhenomenon.h"
#include "..\ForAllQuantities.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {

    DynamicPhenomenon::DynamicPhenomenon(const std::string& phenomenonName)
        : m_dimension(0,0,0,0,0) 
        , m_phenomenonType(typeid(void))
    {
        if (phenomenonName.empty())
            return;
        bool found = false;
        ForAllQuantitiesLambda([&, phenomenonName](auto quantity)
        {
            if (found)
                return;
            if (decltype(quantity)::PhenomenonType::GetName() == phenomenonName)
            {
                this->m_dimension = decltype(quantity)::DimensionType();
                this->m_phenomenonType = typeid(decltype(quantity)::PhenomenonType);
                found = true;
            }
        });
        if (!found)
            throw std::runtime_error(phenomenonName + " is not a recognized phenomenon!");
    }

    DynamicPhenomenon::DynamicPhenomenon()
        : m_dimension(0,0,0,0,0)
        , m_phenomenonType(typeid(void))
    {
    }

    DynamicPhenomenon::DynamicPhenomenon(const std::string& phenomenonName, const DynamicDimension& dimension) 
        : m_dimension(dimension)
        , m_phenomenonType(typeid(void))
    {
        bool found = false;
        ForAllQuantitiesLambda([&, phenomenonName](auto quantity)
        {
            if (found)
                return;
            if (decltype(quantity)::PhenomenonType::GetName() == phenomenonName)
            {
                if(this->m_dimension != decltype(quantity)::DimensionType())
                    throw std::runtime_error(phenomenonName + " is not given the right dimension!");
                this->m_phenomenonType = typeid(decltype(quantity)::PhenomenonType);
                found = true;
            }
        });
        if (!found)
            throw std::runtime_error(phenomenonName + " is not a recognized phenomenon!");
    }

    DynamicPhenomenon::DynamicPhenomenon(const DynamicDimension& dimension)
        : m_dimension(dimension)
        , m_phenomenonType(typeid(void))
    {
    }

    std::string DynamicPhenomenon::GetName() const
    {
        if (m_phenomenonType != typeid(void))
        {
            std::string name = m_phenomenonType.name();
            return name.substr(25, name.size() - 35);
        }
        return "";
    }

    bool DynamicPhenomenon::IsValid() const
    {
        return m_phenomenonType != typeid(void);
    }

    std::list<DynamicPhenomenon> DynamicPhenomenon::GetCompatiblePhenomenons(const DynamicDimension& dimension)
    {
        std::list<DynamicPhenomenon> phenomenons;
        ForAllQuantitiesLambda([&](auto quantity)
        {
            if (dimension == decltype(quantity)::DimensionType())
            {
                phenomenons.push_back(decltype(quantity)::PhenomenonType());
            }
        });
        return phenomenons;
    }

    bool DynamicPhenomenon::operator<(const DynamicPhenomenon& other) const
    {
        return m_phenomenonType < other.m_phenomenonType;
    }

    bool DynamicPhenomenon::operator!=(const DynamicPhenomenon& other) const
    {
        return !operator==(other);
    }

    bool DynamicPhenomenon::operator==(const DynamicPhenomenon& other) const
    {
        return m_phenomenonType == other.m_phenomenonType && m_dimension == other.m_dimension;
    }

}}}}

