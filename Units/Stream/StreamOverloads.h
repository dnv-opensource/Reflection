#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Formatting\FormattingService.h"
#include "Units\Quantity.h"
#include "Units\IPhenomenon.h"

namespace DNVS { namespace MoFa {namespace Units {
    template<class _E, class _Tr, typename T>
    inline std::basic_ostream<_E, _Tr>& __cdecl operator<<(std::basic_ostream<_E, _Tr>& _O, const Quantity<T>& quantity) {
        return _O << ToString(quantity, DNVS::MoFa::Formatting::FormattingService());
    }

}}}