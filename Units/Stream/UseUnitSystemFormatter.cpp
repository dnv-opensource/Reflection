//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "UseUnitSystemFormatter.h"
#include <iostream>

namespace DNVS { namespace MoFa {namespace Units {namespace Stream {

    void UseUnitSystemFormatter::SetToStream( std::ostream& stream, bool flag )
    {
        stream.iword(GetIndex()) = (flag==true);
    }

    bool UseUnitSystemFormatter::GetFromStream( std::ostream& stream )
    {
        return stream.iword(GetIndex()) != 0;
    }

    int UseUnitSystemFormatter::GetIndex()
    {
        static int i = std::ios_base::xalloc();
        return i;
    }

}}}}