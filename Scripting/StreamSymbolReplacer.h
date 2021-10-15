#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "Config.h"
#include "SymbolReplacer.h"
#include <sstream>

namespace DNVS { namespace MoFa { namespace Scripting {

    class SCRIPTING_IMPORT_EXPORT StreamSymbolReplacer
    {
    public:
        StreamSymbolReplacer(SymbolReplacerPointer replacer, size_t strBufSize = 100);
        std::istream& TryReplace(std::istream& stream);

    private:
        bool CheckForSplitConflict(std::istream& stream, char* line, const std::streampos& previousPos);

        SymbolReplacerPointer m_replacer;
        std::stringstream m_replacedStream;
        size_t m_bufSize;
    };

}}}