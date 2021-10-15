#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IScriptingService.h"
#include "Reflection\TypeConversions\TypeConversionService.h"


namespace DNVS {namespace MoFa {namespace Scripting {
    template<typename T>
    T As(Reflection::Variants::Variant& var, const std::shared_ptr<IScriptingService>& scriptingService)
    {
        Reflection::TypeConversions::TypeConversionService typeConversionService(scriptingService->GetConversionGraph());
        return typeConversionService.IntrusiveAs<T>(var);
    }

    template<typename T, typename U>
    typename std::enable_if<std::is_same<T,Reflection::Variants::Variant>::value,Reflection::Variants::Variant>::type As(U&& var)
    {
        return Reflection::Variants::VariantService::Reflect(var);
    }

}}}
