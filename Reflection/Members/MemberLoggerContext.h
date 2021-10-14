#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Config.h"
#include <memory>
#include "TypeUtilities/FunctionDetails/IFunctionDetailService.h"
#include "TypeUtilities/FunctionDetails/IFunctionDetails.h"
#include "Reflection/Types/ITypeFormatter.h"
#include "Reflection/Variants/IVariantFormatter.h"


namespace DNVS {namespace MoFa {namespace Reflection {namespace Members {
    class REFLECTION_IMPORT_EXPORT MemberLoggerContext {
    public:
        MemberLoggerContext();
        MemberLoggerContext(std::shared_ptr<TypeUtilities::IFunctionDetailService> functionDetailService,
            std::shared_ptr<Types::ITypeFormatter> typeFormatter,
            std::shared_ptr<Variants::IVariantFormatter> variantFormatter);

        virtual void SetFunctionDetailService(const std::shared_ptr<TypeUtilities::IFunctionDetailService>& service);
        virtual void SetTypeFormatter(const std::shared_ptr<Types::ITypeFormatter>& formatter);
        virtual void SetVariantFormatter(const std::shared_ptr<Variants::IVariantFormatter>& formatter);

        virtual std::shared_ptr<TypeUtilities::IFunctionDetailService> GetFunctionDetailService() const;
        virtual std::shared_ptr<Types::ITypeFormatter> GetTypeFormatter() const;
        virtual std::shared_ptr<Variants::IVariantFormatter> GetVariantFormatter() const;

    private:
        std::shared_ptr<TypeUtilities::IFunctionDetailService> m_functionDetailService;
        std::shared_ptr<Types::ITypeFormatter> m_typeFormatter;
        std::shared_ptr<Variants::IVariantFormatter> m_variantFormatter;
    };
}}}}