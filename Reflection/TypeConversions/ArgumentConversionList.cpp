//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ArgumentConversionList.h"
#include <algorithm>
#include "ArgumentConversionQuality.h"
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {

    int ArgumentConversionList::Compare(const std::vector<ConversionSequencePointer>& lhs, const std::vector<ConversionSequencePointer>& rhs)
    {
        std::size_t minSize = std::min(lhs.size(), rhs.size());
        std::set<ArgumentConversionQuality> lhsWorst;
        std::set<ArgumentConversionQuality> rhsWorst;

        for (size_t i = 0; i < minSize; ++i)
        {
            auto lhsQuality = lhs[i]->Quality();
            auto rhsQuality = rhs[i]->Quality();
            if (lhsQuality < rhsQuality)
                rhsWorst.insert(rhsQuality);
            else if (rhsQuality < lhsQuality)
                lhsWorst.insert(lhsQuality);
        }
        if (lhsWorst.empty())
            return -1;
        else if (rhsWorst.empty())
            return 1;
        else
        {
            //Prefer conversion without user conversion
            int result = CompareConversions(lhsWorst, rhsWorst, ConversionType::UserConversion);
            if(result == 0)
                //Prefer conversion with reflection conversion.
                result = -CompareConversions(lhsWorst, rhsWorst, ConversionType::ReflectionConversion);
            if (result == 0)
            {
                if (lhs.size() < rhs.size())
                    return -1;
                if (lhs.size() > rhs.size())
                    return 1;
            }
            return result;
        }
    }

    int ArgumentConversionList::CompareConversions(const std::set<ArgumentConversionQuality>& lhsWorst, const std::set<ArgumentConversionQuality>& rhsWorst, ConversionType::Type type)
    {
        ArgumentConversionQuality conversion(type);
        bool lhsHasConversion = lhsWorst.lower_bound(conversion) != lhsWorst.end();
        bool rhsHasConversion = rhsWorst.lower_bound(conversion) != rhsWorst.end();
        if (lhsHasConversion != rhsHasConversion)
        {
            if (rhsHasConversion)
                return -1;
            else
                return 1;
        }
        return 0;
    }

}}}}

