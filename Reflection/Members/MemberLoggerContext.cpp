//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "MemberLoggerContext.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    using namespace DNVS::MoFa::TypeUtilities;
    using namespace Types;
    using namespace Variants;

    MemberLoggerContext::MemberLoggerContext()
        : m_functionDetailService(nullptr)
        , m_typeFormatter(nullptr)
        , m_variantFormatter(nullptr)
    {
    }

    MemberLoggerContext::MemberLoggerContext(std::shared_ptr<TypeUtilities::IFunctionDetailService> functionDetailService, 
        std::shared_ptr<Types::ITypeFormatter> typeFormatter, 
        std::shared_ptr<Variants::IVariantFormatter> variantFormatter)
        : m_functionDetailService(functionDetailService)
        , m_typeFormatter(typeFormatter)
        , m_variantFormatter(variantFormatter)
    {

    }

    void MemberLoggerContext::SetFunctionDetailService(const std::shared_ptr<IFunctionDetailService>& service)
    {
        m_functionDetailService = service;
    }

    void MemberLoggerContext::SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& formatter)
    {
        m_typeFormatter = formatter;
    }

    void MemberLoggerContext::SetVariantFormatter(const std::shared_ptr<Variants::IVariantFormatter>& formatter)
    {
        m_variantFormatter = formatter;
    }

    std::shared_ptr<IFunctionDetailService> MemberLoggerContext::GetFunctionDetailService() const
    {
        return m_functionDetailService;
    }

    std::shared_ptr<Types::ITypeFormatter> MemberLoggerContext::GetTypeFormatter() const
    {
        return m_typeFormatter;
    }

    std::shared_ptr<Variants::IVariantFormatter> MemberLoggerContext::GetVariantFormatter() const
    {
        return m_variantFormatter;
    }

}}}}