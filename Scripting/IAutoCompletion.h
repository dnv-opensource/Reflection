#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <set>
#include <string>

namespace DNVS { namespace MoFa {namespace Scripting {
    class IAutoCompletion
    {
    public:
        virtual ~IAutoCompletion() {}
        virtual std::set<std::string> GetFunctionSet() const = 0;
        virtual std::set<std::string> GetVariableSet() const = 0;
        virtual bool IsInFunction() const = 0;
        virtual std::string GetFunctionName() const = 0;
        virtual int GetFunctionStartPosition() const = 0;
        virtual int GetStartOfCompletion() const = 0;
    };
}}}