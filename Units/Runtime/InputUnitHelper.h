#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IInputUnitProvider.h"
#include <memory>
#include "IUnitProvider.h"

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
    class InputUnitHelper {
    public:
        InputUnitHelper(const std::shared_ptr<IInputUnitProvider>& inputUnitProvider)
            : m_inputUnitProvider(inputUnitProvider)
        {}
        template<typename QuantityT, typename ActualQuantityT>
        void SetUnits(ActualQuantityT conversionFactor)
        {
            using DimensionType = QuantityT::DimensionType;
            using PhenomenonType = QuantityT::PhenomenonType;
            static_assert(std::is_same_v<DimensionType, ActualQuantityT::DimensionType>, "The input quantity must match the input unit");
            m_inputUnitProvider->GetUnitProvider()->SetDimensionUnit(DimensionType(), Runtime::Unit("", conversionFactor.GetValue(), DimensionType()));
            m_inputUnitProvider->GetUnitProvider()->SetPhenomenonUnit(PhenomenonType(), Runtime::Unit("", conversionFactor.GetValue(), DimensionType()));
        }
    private:
        std::shared_ptr<IInputUnitProvider> m_inputUnitProvider;
    };
}}}}