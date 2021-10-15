//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "StreamSymbolReplacer.h"
#include "ISymbolReplacerService.h"
#include "Services/ServiceProvider.h"

namespace DNVS { namespace MoFa { namespace Scripting {

    StreamSymbolReplacer::StreamSymbolReplacer(SymbolReplacerPointer replacer, size_t strBufSize)
        : m_replacer(replacer)
        , m_bufSize(strBufSize)
    {

    }

    std::istream& StreamSymbolReplacer::TryReplace(std::istream& stream)
    {
        if (!m_replacer)
            return stream;

        char line[100];
        while (stream.good())
        {
            std::streampos prevPos = stream.tellg();
            stream.read(line, m_bufSize);
            bool canReplace = !CheckForSplitConflict(stream, line, prevPos);
            if (canReplace)
            {
                std::string& replaced = m_replacer->Replace(line, stream.gcount());
                m_replacedStream.write(replaced.c_str(), replaced.size());
            }
        }
        return m_replacedStream;
    }

    bool StreamSymbolReplacer::CheckForSplitConflict(std::istream& stream, char* line, const std::streampos& previousPos)
    {
        if (stream.gcount() == m_bufSize)
        {
            char *pos = strstr(line, m_replacer->GetSymbol().c_str());
            if (pos)
            {
                // if the symbol is too close to the split point, then fallback to replacing the entire line
                size_t index = pos - line;
                if (index > m_bufSize - 7)
                {
                    stream.seekg(previousPos);
                    std::string strLine;
                    std::getline(stream, strLine);
                    std::string& replacedStr = m_replacer->Replace(strLine);
                    m_replacedStream.write(replacedStr.c_str(), replacedStr.size());
                    return true;
                }
            }
        }

        return false;
    }

}}}