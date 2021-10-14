//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "InsertSpaceBeforeCapitalLetters.h"

namespace DNVS { namespace MoFa { namespace Formatting { 

    std::string InsertSpaceBeforeCapitalLetters::Format(const std::string& text) const
    {
        std::string result = text;
        bool priorUpper = true;
        for (std::string::iterator it = result.begin(); it != result.end(); it++) {
            char letter = *it;
            if ((letter >= 'A' && letter <= 'Z') || letter == '_') {
                if (!priorUpper)
                {
                    it = result.insert(it, ' ');
                    it++;
                }
                else priorUpper = true;
            }
            else priorUpper = false;
        }
        return result;
    }

    IStringFormatter* InsertSpaceBeforeCapitalLetters::Clone() const
    {
        return new InsertSpaceBeforeCapitalLetters;
    }

}}}


