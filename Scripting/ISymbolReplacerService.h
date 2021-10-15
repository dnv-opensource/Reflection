#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "SymbolReplacer.h"

namespace DNVS {namespace MoFa {namespace Scripting {

    class ISymbolReplacerService
    {
    public:
        virtual SymbolReplacerPointer GetSymbolReplacer() const = 0;
    };

    typedef std::shared_ptr<ISymbolReplacerService> SymbolReplacerServicePointer;

}}}