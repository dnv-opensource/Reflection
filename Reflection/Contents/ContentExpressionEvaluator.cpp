//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ContentExpressionEvaluator.h"
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/classification.hpp"

namespace DNVS {namespace MoFa {namespace Reflection { namespace Contents { 

    ContentExpressionEvaluator::ContentExpressionEvaluator(const std::shared_ptr<IReflectionContentsService>& service)
        : m_service(service)
    {

    }

    Objects::Object ContentExpressionEvaluator::EvaluateExpression(const std::string& text) const noexcept
    {
        try {
            std::string script = boost::algorithm::trim_right_copy_if(text, boost::algorithm::is_space());

            if (script.empty())
                return Objects::Object();
            else if (script.back() != ';' && script.back() != '}')
                script += ';';
            return m_service->Test(script);
        }
        catch (...)
        {
            return Reflection::Objects::Object();
        }
    }

    TypeLibraries::TypeLibraryPointer ContentExpressionEvaluator::GetTypeLibrary() const
    {
        return m_service->GetTypeLibrary();
    }

}}}}

