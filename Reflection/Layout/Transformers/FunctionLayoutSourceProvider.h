#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ILayoutSourceProvider.h"
#include "Reflection/TypeLibraries/TypeLibraryPointer.h"
#include "Reflection/Members/IMember.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Layout {
    class FunctionLayoutSourceProvider
        : public ILayoutSourceProvider
    {
    public:
        FunctionLayoutSourceProvider(const TypeLibraries::TypeLibraryPointer& typeLibrary, Members::MemberPointer function);
        virtual std::shared_ptr<ILayoutSource> LookupLayoutSource(const std::string& name) override;
    private:
        TypeLibraries::TypeLibraryPointer m_typeLibrary;
        Members::MemberPointer m_function;
    };
}}}}
