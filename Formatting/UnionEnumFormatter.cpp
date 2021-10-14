//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UnionEnumFormatter.h"

namespace DNVS { namespace MoFa { namespace Formatting {

    UnionEnumFormatter::UnionEnumFormatter(const std::shared_ptr<IEnumFormatter>& lhs, const std::shared_ptr<IEnumFormatter>& rhs)
    {
        m_formatters.push_back(lhs);
        m_formatters.push_back(rhs);
    }

    std::string UnionEnumFormatter::FormatEnum(int enumValue, const std::type_info& enumType, const FormattingService& service) const
    {
        std::string bestMatch = "N/A";
        for(const auto& formatter : m_formatters)
        {
            if (formatter->IsSupported(enumType))
            {
                bestMatch = formatter->FormatEnum(enumValue, enumType, service);
                if (!bestMatch.empty())
                    break;
            }
        }
        return bestMatch;
    }

    bool UnionEnumFormatter::IsSupported(const std::type_info& info) const
    {
        for(const auto& formatter : m_formatters)
        {
            if (formatter->IsSupported(info))
                return true;                
        }
        return false;
    }

}}}