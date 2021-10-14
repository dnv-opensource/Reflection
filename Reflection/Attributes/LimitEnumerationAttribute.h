#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "FunctionWrapper.h"
#include <functional>

namespace DNVS {namespace MoFa {namespace Reflection {namespace Attributes {
    class LimitEnumerationAttribute {
    public:
        template<typename Callback>
        LimitEnumerationAttribute(Callback callback)
            :m_callback(FunctionWrapper<Callback, bool>(callback))
        {
            static_assert(TypeUtilities::FunctionTraits<Callback>::Arity == 1, "Limit function can only take one argument");
        }
        bool IsIncluded(const Objects::Object& object) const
        {
            return m_callback({ object });
        }
    private:
        std::function<bool(const std::vector<Objects::Object>&)> m_callback;
    };
}}}}