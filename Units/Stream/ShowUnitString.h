#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "..\Config.h"
#include <iosfwd>

namespace DNVS { namespace MoFa {namespace Units {namespace Stream {
    class UNITS_IMPORT_EXPORT ShowUnitString 
    {
    public:
        static void SetToStream(std::ostream& stream, bool flag);
        static bool GetFromStream(std::ostream& stream);
    private:
        static int GetIndex();
    };

}}}}