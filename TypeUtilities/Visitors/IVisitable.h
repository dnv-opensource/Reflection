#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "IVisitor.h"

namespace DNVS {namespace MoFa {namespace TypeUtilities {
    class IVisitable {
    public:
        virtual ~IVisitable() {}
        virtual void Accept(IVisitor& visitor) const = 0;
    protected:
        template<typename T>
        static bool AcceptImpl(const T& visited,IVisitor& visitor) {
            if(TypedVisitor<T>* p=dynamic_cast<TypedVisitor<T>*>(&visitor)) {
                p->Visit(visited);
                return true;
            }
            return false;
        }
    };

}}}