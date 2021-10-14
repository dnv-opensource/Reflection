//  Copyright (c) 2021 DNV AS
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
#include "ThreadPool.h"



namespace DNVS {namespace MoFa { namespace TypeUtilities {

    ThreadPool::ThreadPool(size_t threadCount)
        : m_terminate_pool(false)
    {
        m_threads.reserve(threadCount);
        for (size_t i = 0; i < threadCount; ++i)
        {
            m_threads.push_back(std::thread([&]() {RunThread(); }));
        }
    }

    ThreadPool::ThreadPool()
        : ThreadPool(std::thread::hardware_concurrency())
    {
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_terminate_pool = true; 
        } 

        m_condition.notify_all(); // wake up all threads.

        // Join all threads.
        for (std::thread &every_thread : m_threads)
        {
            every_thread.join();
        }
        m_threads.clear();
    }

    void ThreadPool::AddTask(const std::function<void()>& task)
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_queue.push(task);
        }
        m_condition.notify_one();
    }

    void ThreadPool::FlushAndAddTask(const std::function<void()>& task)
    {
        std::unique_lock<std::mutex> lock(m_queueMutex);
        while(!m_queue.empty())
            m_queue.pop();
        m_queue.push(task);
        m_condition.notify_one();
    }

    void ThreadPool::Wait(const std::function<bool()>& pred)
    {
        while (!pred())
            RunSingleJob(false);
    }

    void ThreadPool::RunThread()
    {
        while (!m_terminate_pool)
        {
            RunSingleJob(true);
        }
    }

    void ThreadPool::RunSingleJob(bool allowWait)
    {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);

            if(allowWait)
                m_condition.wait(lock, [&] {return !m_queue.empty() || m_terminate_pool; });
            if (!m_queue.empty())
            {
                job = m_queue.front();
                m_queue.pop();
            }
        }
        if (job)
            job();
    }

}}}