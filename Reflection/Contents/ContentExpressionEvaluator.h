#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IReflectionContentsService.h"
#include "Reflection/IExpressionEvaluator.h"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 
    class ContentExpressionEvaluator : public IExpressionEvaluator
    {
    public:
        ContentExpressionEvaluator(const std::shared_ptr<IReflectionContentsService>& service);
        virtual Objects::Object EvaluateExpression(const std::string& text) const noexcept override;
        virtual TypeLibraries::TypeLibraryPointer GetTypeLibrary() const override;
    private:
        std::shared_ptr<IReflectionContentsService> m_service;
    };
}}}}