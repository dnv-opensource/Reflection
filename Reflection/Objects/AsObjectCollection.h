#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Object.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Objects {
    /**
    If input object is of type CollectionType, iterate through all its elements and call a customized function.
    Otherwise pass object to the customized function.
    */
    template<typename CollectionType>
    class AsObjectCollection {
    public:
        AsObjectCollection(const Object& object) 
        :   m_object(object)
        {}
        void ForEach(std::function<void(Object)> function) {
            if(m_object.IsConvertibleTo<const CollectionType&>()) {
                const CollectionType& collection=m_object.As<const CollectionType&>();
                std::for_each(collection.begin(),collection.end(),function);
            }
            else {
                function(m_object);
            }
        }
    private:
        Objects::Object m_object;
    };
}}}}
