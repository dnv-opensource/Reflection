#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "Reflection/Objects/Object.h"

namespace DNVS {namespace MoFa {namespace Reflection {namespace Utilities {    
    class CallableConstructor;
    class REFLECTION_IMPORT_EXPORT ConstructorSelector {
    public:
        ConstructorSelector(const Objects::Object& object, bool checkRecursive = true);
        CallableConstructor GetBestConstructor();
        CallableConstructor GetBestConstructor(const std::set<Members::MemberPointer>& members) const;

        template<typename T>
        static CallableConstructor TryGetBestConstructor(T* arg)
        {
            auto typeService = DNVS::MoFa::Services::ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
            ConstructorSelector constructorSelector(Objects::Object::CreateDynamic(typeService, arg));
            return constructorSelector.GetBestConstructor();
        }
        template<typename T>
        static CallableConstructor TryGetBestConstructor(const T& arg)
        {
            auto typeService = ServiceProvider::Instance().GetService<TypeLibraries::ITypeLibrary>();
            ConstructorSelector constructorSelector(Objects::Object::CreateDynamic(typeService, arg));
            return constructorSelector.GetBestConstructor();
        }
    private:
        Objects::Object m_object;
        bool m_checkRecursive;
    };
}}}}