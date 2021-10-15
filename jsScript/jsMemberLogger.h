#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <time.h>
#include <memory>
#include <string>
#include "jsScript_config.h"

namespace DNVS { namespace MoFa { namespace TypeUtilities {
    class IFunctionDetailService;
}}}
class jsMember;

class JSSCRIPT_IMPORT_EXPORT jsMemberLogger {
public:
    jsMemberLogger(jsMember* member, bool logCall = true);
    jsMemberLogger(jsMember* member, const std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService>& service, bool logCall = true);

    void Initialize();

    ~jsMemberLogger();
    std::string CreateSignature() const;
    std::string AddArguments() const;

private:
    jsMember* m_member;
    clock_t m_start;
    std::string m_dll;
    std::string m_class;
    std::string m_function;
    std::string m_signature;
    std::shared_ptr<DNVS::MoFa::TypeUtilities::IFunctionDetailService> m_service;
    bool m_logCall;
};
