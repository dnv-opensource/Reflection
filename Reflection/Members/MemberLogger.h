#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <time.h>
#include <memory>
#include <string>
#include "Reflection/Config.h"
#include "IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    class MemberLoggerContext;

    class REFLECTION_IMPORT_EXPORT MemberLogger {
    public:
        MemberLogger(IMember* member, bool logCall = true);
        ~MemberLogger();

    private:
        IMember* m_member;
        bool m_logCall;
    };
}}}}