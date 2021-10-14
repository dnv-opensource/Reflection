#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <stdexcept>
#include <list>
#include <set>
#include <vector>

namespace DNVS { namespace MoFa { namespace TypeUtilities { namespace Bridge {
    template<typename ClassT>
    struct ImplementorType {
        static ClassT& GetInstance();
        typedef decltype(GetInstance().GetImplementor()) type;
    };
    template<typename DerivedT, typename BaseT>
    typename ImplementorType<DerivedT>::type GetDerivedImplementor(const BaseT& base)
    {
        typedef typename ImplementorType<DerivedT>::type DerivedImplementorPointer;
        typedef typename DerivedImplementorPointer::element_type DerivedImplementorType;
        using namespace std;
        return DerivedT::GetDerivedImplementor(base);
    }
    template<typename DerivedT, typename BaseT>
    bool Is(const BaseT& base)
    {
        return GetDerivedImplementor<DerivedT>(base) != nullptr;
    }

    template<typename DerivedT, typename BaseT>
    DerivedT As(const BaseT& base)
    {
        auto derivedImplementor = GetDerivedImplementor<DerivedT>(base);
        if(derivedImplementor)
            return DerivedT(derivedImplementor);
        else
            throw std::runtime_error("Unable to cast " + std::string(typeid(BaseT).name()) + " to " + std::string(typeid(DerivedT).name()));
    }

    template<typename DerivedT, typename BaseT>
    DerivedT TryAs(const BaseT& base)
    {
        auto derivedImplementor = GetDerivedImplementor<DerivedT>(base);
        return DerivedT(derivedImplementor);
    }

    template<typename T>
    std::list<typename ImplementorType<T>::type> GetImplementorCollection(const std::list<T>& collection)
    {
        std::list<typename ImplementorType<T>::type> implementorCollection;
        for(auto ivalue = collection.begin(); ivalue != collection.end(); ++ivalue)
            implementorCollection.push_back(ivalue->GetImplementor());
        return implementorCollection;
    }

    template<typename T, typename TImplementor>
    std::list<T> GetCollectionFromImplementors(const std::list<TImplementor>& collection)
    {
        return std::list<T>(collection.begin(), collection.end());
    }

    template<typename T>
    std::set<typename ImplementorType<T>::type> GetImplementorCollection(const std::set<T>& collection)
    {
        std::set<typename ImplementorType<T>::type> implementorCollection;
        for(auto ivalue = collection.begin(); ivalue != collection.end(); ++ivalue)
            implementorCollection.push_back(ivalue->GetImplementor());
        return implementorCollection;
    }

    template<typename T, typename TImplementor>
    std::set<T> GetCollectionFromImplementors(const std::set<TImplementor>& collection)
    {
        return std::set<T>(collection.begin(), collection.end());
    }

    template<typename T>
    std::vector<typename ImplementorType<T>::type> GetImplementorCollection(const std::vector<T>& collection)
    {
        std::vector<typename ImplementorType<T>::type> implementorCollection;
        for(auto ivalue = collection.begin(); ivalue != collection.end(); ++ivalue)
            implementorCollection.push_back(ivalue->GetImplementor());
        return implementorCollection;
    }

    template<typename T, typename TImplementor>
    std::vector<T> GetCollectionFromImplementors(const std::vector<TImplementor>& collection)
    {
        return std::vector<T>(collection.begin(), collection.end());
    }
}}}}