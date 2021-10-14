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

    class REFLECTION_IMPORT_EXPORT MemberLoggerImpl {
    public:
        MemberLoggerImpl(IMember* member, bool logCall = true);
        MemberLoggerImpl(IMember* member, const std::shared_ptr<Members::MemberLoggerContext>&, bool logCall = true);
        ~MemberLoggerImpl();
        std::string GetDLL() { return m_dll; };
        std::string GetClass() { return m_class; };
        std::string GetFunction() { return m_function; };
        std::string GetSignature() { return m_signature; };


        std::string CreateMemberFunctionSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreateStaticFunctionSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreateFunctionSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreateConstructorSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreatePropertyGetSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreatePropertySetSignature(MemberLoggerContext* context = nullptr) const;
        std::string CreateOperatorSignature(MemberLoggerContext* context = nullptr) const;

    private:
        IMember* m_member;
        clock_t m_start;
        std::string m_dll;
        std::string m_class;
        std::string m_function;
        std::string m_signature;
        bool m_logCall;

        void Initialize();
    };
}}}}