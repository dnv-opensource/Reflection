#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IConversionSequence.h"
#include "Reflection/Config.h"
#include <vector>
#include <set>

namespace DNVS {namespace MoFa {namespace Reflection { namespace TypeConversions {
    class REFLECTION_IMPORT_EXPORT ArgumentConversionList {
    public:
        static int Compare(const std::vector<ConversionSequencePointer>& lhs, const std::vector<ConversionSequencePointer>& rhs);
        static int CompareConversions(const std::set<ArgumentConversionQuality>& lhsWorst, const std::set<ArgumentConversionQuality>& rhsWorst, ConversionType::Type type);
    };
}}}}