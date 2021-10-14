//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberLogger.h"
#include "MemberLoggerContext.h"
#include "MemberLoggerImpl.h"
#include <iostream>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
MemberLogger::MemberLogger(IMember* member, bool logCall)
    : m_member(member)
    , m_logCall(logCall)
{
}

MemberLogger::~MemberLogger()
{
    try
    {
        if (m_member->GetMemberLoggerContext() && m_member->GetMemberLoggerContext()->GetFunctionDetailService() != nullptr && !std::uncaught_exception())
        {
            MemberLoggerImpl(m_member, m_logCall);
        }
    }
   catch (std::exception& e) {
        std::cerr << e.what();
    }
    catch (...) {
        std::cerr << "Unknown error during member logging";
    }
}
}}}}
