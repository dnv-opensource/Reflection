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
#include "IThreadPool.h"

namespace DNVS {namespace MoFa { namespace TypeUtilities {
    class ThreadPool : public IThreadPool {
    public:
        ThreadPool(size_t threadCount);
        ThreadPool();
        ~ThreadPool();
        void AddTask(const std::function<void()>& task) override;
        void FlushAndAddTask(const std::function<void()>& task);
        void Wait(const std::function<bool()>& pred) override;
    private:
        void RunThread();
        void RunSingleJob(bool allowWait);
        std::mutex m_queueMutex;
        std::condition_variable m_condition;
        std::queue<std::function<void()>> m_queue;
        std::vector<std::thread> m_threads;
        bool m_terminate_pool;
    };
}}}