#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <algorithm>
#include <ppl.h>
#include <thread>
#include <vector>
namespace DNVS {namespace MoFa {namespace TypeUtilities {
    template<typename It, typename Fn>
    void ParallelForEach(It first, It last, const Fn& fn)
    {
        size_t numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        size_t dist = std::distance(first, last);
        if (dist == 0)
            return;
        if (dist < numThreads)
            numThreads = dist;
        size_t segmentLength = dist / numThreads;
        threads.reserve(numThreads);

        It current = first;
        for (size_t iThread = 0; iThread < numThreads - 1; ++iThread)
        {
            It next = std::next(current, segmentLength);
            threads.emplace_back(std::thread([=, &fn]() {std::for_each(current, next, fn);  }));
            current = next;
        }
        threads.emplace_back(std::thread([=, &fn]() {std::for_each(current, last, fn);  }));
        for (auto& th : threads) {
            th.join();
        }
    }

    template<typename It,typename Fn>
    void ForEach(bool allowConcurrent, It first, It last, const Fn& fn)
    {
        if (allowConcurrent)            
            ParallelForEach(first, last, fn);
        else
            std::for_each(first, last, fn);
    }
}}}