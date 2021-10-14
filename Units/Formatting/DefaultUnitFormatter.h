#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IUnitFormatter.h"
#include <memory>
#include "Units\Config.h"
namespace DNVS { namespace MoFa { namespace Units { namespace Runtime {
    //Need to forward declare this class to avoid circular dependency
    class UnitSystem;
}}}}

namespace DNVS { namespace MoFa { namespace Units { namespace Formatting {
    class IUnitFormatterService;
#pragma warning(push)
#pragma warning(disable:4275)
    class UNITS_IMPORT_EXPORT DefaultUnitFormatter : public IUnitFormatter
    {
    public:        
        DefaultUnitFormatter(bool includeUnitString = true, bool getPrimitiveFormatterFromPhenomenon = false);
        DefaultUnitFormatter(const Runtime::UnitSystem& unitSystem, bool includeUnitString = true, bool getPrimitiveFormatterFromPhenomenon = false);
        ~DefaultUnitFormatter();
        virtual std::string FormatQuantity(double number, const Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
        virtual std::string FormatQuantity(double number, const Runtime::DynamicDimension& dimension, const DNVS::MoFa::Formatting::FormattingService& formattingService) const override;
    private:
        std::unique_ptr<Runtime::UnitSystem> m_unitSystem;
        std::shared_ptr<IUnitFormatterService> m_formatter;
        //Includes the unit string in the returned text, "45 mm" instead of "45".
        bool m_includeUnitString;
        //If true, we will use the formatting rules from the phenomenon when formatting the number, so that length and mass may be formatted differently, for example.
        bool m_getPrimitiveFormatterFromPhenomenon;
    };
#pragma warning(pop)
}}}}

