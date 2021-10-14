//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "DimensionFormatter.h"
#include <stdio.h>
#include "..\Runtime\DynamicDimension.h"

namespace DNVS { namespace MoFa { namespace Units { namespace Formatting {

    std::string DimensionFormatter::FormatAsHtml(const Runtime::DynamicDimension& dimension) const
    {
        bool theFirst = true;
        std::string signature;
        AppendHtmlComponent(signature, 'A', dimension.GetAngle());
        AppendHtmlComponent(signature, 'F', dimension.GetForce());
        AppendHtmlComponent(signature, 'L', dimension.GetLength());
        AppendHtmlComponent(signature, 'O', dimension.GetTemperatureDifference());
        AppendHtmlComponent(signature, 'T', dimension.GetTime());
        return signature;
    }

    std::string DimensionFormatter::GetHtmlComponent(char lit, int exponent) const
    {
        if (exponent == 0)
            return "";
        return lit + GetHtmlExponent(exponent);
    }

    std::string DimensionFormatter::GetHtmlExponent(int exponent) const
    {
        if (exponent == 1)
            return "";
        char numberAsString[200];
        sprintf_s(numberAsString, 200, "<sup>%i</sup>", exponent);
        return numberAsString;
    }

    void DimensionFormatter::AppendHtmlComponent(std::string& signature, char lit, int exponent) const
    {
        std::string component = GetHtmlComponent(lit, exponent);
        if(!component.empty() && !signature.empty())
            signature += "&bull;";
        signature += component;
    }

}}}}