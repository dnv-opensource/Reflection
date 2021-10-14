#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace DNVS {namespace MoFa {namespace Units {

    template<typename LambdaT>
    class LambdaApplier
    {
    public:
        LambdaApplier(const LambdaT& lambda) : m_lambda(lambda) {}
        template<typename T>
        void Apply()
        {
            m_lambda(T());
        }
    private:
        LambdaT m_lambda;
    };
}}}    