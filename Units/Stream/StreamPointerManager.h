#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <iostream>

namespace DNVS { namespace MoFa {namespace Units {namespace Stream {
    template<typename StreamIndexerT, typename T, int PointerSize = sizeof(T*)>
    class StreamPointerManager;

    template<typename StreamIndexerT, typename T>
    class StreamPointerManager<StreamIndexerT, T, 4>
    {
        union PointerBuilder {
            int value1;
            T* pointer;
        };
    public:
        static T* Get(std::ostream& stream)
        {
            PointerBuilder builder;
            builder.value1 = stream.iword(StreamIndexerT::GetPointerIndex1());
            return builder.pointer;
        }
        static void Set(std::ostream& stream, T* system)
        {
            PointerBuilder builder;
            builder.pointer = system;
            stream.iword(StreamIndexerT::GetPointerIndex1()) = builder.value1;
        }
    };

    template<typename StreamIndexerT, typename T>
    class StreamPointerManager<StreamIndexerT, T, 8>
    {
    public:
        union PointerBuilder {
            struct {
                int value1, value2;
            };
            T* pointer;
        };
        static T* Get(std::ostream& stream)
        {
            PointerBuilder builder;
            builder.value1 = stream.iword(StreamIndexerT::GetPointerIndex1());
            builder.value2 = stream.iword(StreamIndexerT::GetPointerIndex2());
            return builder.pointer;
        }
        static void Set(std::ostream& stream, T* system)
        {
            PointerBuilder builder;
            builder.pointer = system;
            stream.iword(StreamIndexerT::GetPointerIndex1()) = builder.value1;
            stream.iword(StreamIndexerT::GetPointerIndex2()) = builder.value2;
        }
    };
}}}}