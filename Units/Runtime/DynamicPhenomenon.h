#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DynamicDimension.h"
#include <string>
#include <list>
#include <typeindex>

namespace DNVS {namespace MoFa {namespace Units { 
    class IPhenomenon;
}}}
namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class UNITS_IMPORT_EXPORT DynamicPhenomenon {
    public:
        DynamicPhenomenon();
        //Parse the dimension based on the phenomenon name. Throws if phenomenon cannot be found.
        DynamicPhenomenon(const std::string& phenomenonName);
        DynamicPhenomenon(const std::string& phenomenonName, const DynamicDimension& dimension);
        DynamicPhenomenon(const DynamicDimension& dimension);
        template<typename PhenomenonT>
        DynamicPhenomenon(const PhenomenonT& phenomenon, std::enable_if_t<std::is_base_of_v<IPhenomenon,PhenomenonT>,void*> = nullptr)
            : m_dimension(typename PhenomenonT::DimensionType())
            , m_phenomenonType(typeid(PhenomenonT))
        {}
        std::string GetName() const;
        const DynamicDimension& GetDimension() const { return m_dimension; }
        bool IsValid() const;
        static std::list<DynamicPhenomenon> GetCompatiblePhenomenons(const DynamicDimension& dimension);
        bool operator==(const DynamicPhenomenon& other) const;
        bool operator!=(const DynamicPhenomenon& other) const;
        bool operator<(const DynamicPhenomenon& other) const;
    private:
        DynamicDimension m_dimension;
        std::type_index m_phenomenonType;
    };

}}}}