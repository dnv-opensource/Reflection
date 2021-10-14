#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnitProvider.h"
#include <map>
#include "..\Config.h"
#include <mutex>

namespace DNVS {namespace MoFa {namespace Units { namespace Runtime {
#pragma warning(push)
#pragma warning(disable:4275) //non dll-interface class 'Base' used as base for dll-interface class 'Class'
    class UNITS_IMPORT_EXPORT DefaultUnitProvider : public IUnitProvider
    {
    public:
        DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time, const Unit& angle);
        DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff, const Unit& time);
        DefaultUnitProvider(const Unit& length, const Unit& force, const Unit& tempDiff);
        DefaultUnitProvider(const Unit& length, const Unit& force);
        DefaultUnitProvider();
        virtual Unit GetDimensionUnit(const DynamicDimension& dimension) override;
        virtual Unit GetPhenomenonUnit(const DynamicPhenomenon& phenomenon) override;
        virtual void SetDimensionUnit(const DynamicDimension& dimension, const Unit& unit) override;
        virtual void SetPhenomenonUnit(const DynamicPhenomenon& phenomenon, const Unit& unit) override;
        virtual void Clear() override;
        virtual std::map<DynamicDimension, Unit> GetDimensionUnits() const override;
        virtual std::map<DynamicPhenomenon, Unit> GetPhenomenonUnits() const override;

        std::shared_ptr<IUnitProvider> GetCache() const override;
        virtual void ClearCache() override;
    private:
        std::map<DynamicDimension, Unit> m_dimensions;
        std::map<DynamicPhenomenon, Unit> m_phenomenons;
        mutable std::shared_ptr<IUnitProvider> m_cache;
        mutable std::mutex m_mutex;
    };
#pragma warning(pop)
}}}}