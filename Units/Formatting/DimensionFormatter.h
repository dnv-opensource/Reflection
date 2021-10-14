#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Units\Config.h"
#include <string>
namespace DNVS { namespace MoFa { namespace Units { namespace Runtime {
    //Need to forward declare this class to avoid circular dependency
    class DynamicDimension;
}}}}

namespace DNVS { namespace MoFa { namespace Units { namespace Formatting {
    class UNITS_IMPORT_EXPORT DimensionFormatter
    {
    public:        
        std::string FormatAsHtml(const Runtime::DynamicDimension& dimension) const;
    private:
        std::string GetHtmlComponent(char lit, int exponent) const;
        std::string GetHtmlExponent(int exponent) const;
        void AppendHtmlComponent(std::string& signature, char lit, int exponent) const;
    };
}}}}