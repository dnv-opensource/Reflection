#pragma once
//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <concrt.h>

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    class IThreadPool {
    public:
        ~IThreadPool() {}
        virtual void AddTask(const std::function<void()>& task) = 0;
        virtual void Wait(const std::function<bool()>& pred) = 0;
        template<typename It, typename Fn>
        void ForEach(It first, It last, const Fn& fn)
        {
            std::atomic<size_t> counter = 0;
            size_t totalCounter = 0;
            for (; first != last; ++first)
            {
                ++totalCounter;
                AddTask([=,&counter,&fn]() {fn(*first); ++counter; });
            }
            Wait([&]() {return counter == totalCounter; });
        }
    private:
    };
}}}