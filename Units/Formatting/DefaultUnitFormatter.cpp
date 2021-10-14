//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DefaultUnitFormatter.h"
#include "Units\Runtime\DatabaseUnitSystem.h"
#include "Formatting\DefaultPrimitiveFormatter.h"
#include "Services\ServiceProvider.h"
#include "IUnitFormatterService.h"

namespace DNVS { namespace MoFa { namespace Units { namespace Formatting {

    DefaultUnitFormatter::DefaultUnitFormatter(bool includeUnitString, bool getPrimitiveFormatterFromPhenomenon)
        : DefaultUnitFormatter(Runtime::GetInputUnitSystem(), includeUnitString, getPrimitiveFormatterFromPhenomenon)
    {
    }

    DefaultUnitFormatter::DefaultUnitFormatter(const Runtime::UnitSystem& unitSystem, bool includeUnitString, bool getPrimitiveFormatterFromPhenomenon)
        : m_unitSystem(std::make_unique<Runtime::UnitSystem>(unitSystem))
        , m_includeUnitString(includeUnitString)
        , m_getPrimitiveFormatterFromPhenomenon(getPrimitiveFormatterFromPhenomenon)
    {
        if (m_getPrimitiveFormatterFromPhenomenon)
            m_formatter = Services::ServiceProvider::Instance().TryGetService<IUnitFormatterService>();

    }

    DefaultUnitFormatter::~DefaultUnitFormatter()
    {

    }

    std::string DefaultUnitFormatter::FormatQuantity(double number, const Runtime::DynamicPhenomenon& phenomenon, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {      
        //Get the default unit for this phenomenon, for example in input units. (e.g. mm)
        Runtime::Unit unit = m_unitSystem->GetUnit(phenomenon);
        //The input number will always be in standard units, such as m, N, kg etc.
        double numberToFormat = number / unit.GetConversionFactor();
        //The different phenomena may have different formatting rules, for example: length should have 6 significant digits while
        //mass should have 3. Sometimes we want to use this information when printing our numbers. For example in reports.
        std::shared_ptr<DNVS::MoFa::Formatting::IPrimitiveFormatter> primitiveFormatter;
        if (m_formatter)
            primitiveFormatter = m_formatter->GetPrimitiveFormatter(phenomenon);
        if(!primitiveFormatter)
            primitiveFormatter = formattingService.GetFormatterOrDefault<DNVS::MoFa::Formatting::IPrimitiveFormatter, DNVS::MoFa::Formatting::DefaultPrimitiveFormatter>();
        std::string text = primitiveFormatter->FormatDouble(numberToFormat, formattingService);
        if (m_includeUnitString)
            return std::string(text) + " " + unit.GetUnitName();
        else
            return text;
    }

    std::string DefaultUnitFormatter::FormatQuantity(double number, const Runtime::DynamicDimension& dimension, const DNVS::MoFa::Formatting::FormattingService& formattingService) const
    {
        Runtime::Unit unit = m_unitSystem->GetUnit(dimension);
        double numberToFormat = number / unit.GetConversionFactor();
        auto primitiveFormatter = formattingService.GetFormatterOrDefault<DNVS::MoFa::Formatting::IPrimitiveFormatter, DNVS::MoFa::Formatting::DefaultPrimitiveFormatter>();
        std::string text = primitiveFormatter->FormatDouble(numberToFormat, formattingService);
        if (m_includeUnitString)
            return std::string(text) + " " + unit.GetUnitName();
        else
            return text;
    }

}}}}