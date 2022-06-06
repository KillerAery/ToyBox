#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <iostream>
#include <functional>
#include "Job.h"
#include "LockfreeQueue.h"

class ThreadPool
{
public:

    explicit ThreadPool(int num) : _thread_num(num), _is_running(false), _len(0) {}

    ~ThreadPool() {
        if (_is_running)
            stop();
    }

    void start() {
        _is_running = true;

        //开启线程
        for (int i = 0; i < _thread_num; ++i)
            _threads.emplace_back(std::thread(&ThreadPool::work, this));
    }

    void stop() {
        {
            //因为有的线程可能处于阻塞状态,所以在回收线程之前保证所有线程均处于运行状态
            std::unique_lock<std::mutex> lk(_mtx);
            _is_running = false;
            _cond.notify_all();
        }

        for (auto& thread : _threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    //往任务队列添加任务
    void appendTask(Job* job) {
        if (_is_running) {
            _queue.push(job);
            _len.fetch_add(1);
            _cond.notify_one();
        }
    }

    template<class U>
    void setFinishFunc(U && func){
        _finishedOneJob = std::forward<U>(func);
    }

private:
    //工作接口
    void work() {
        while (_is_running)
        {
            // 从 Task Queue 中取出一个 task
            Job* task = takeOneTask();
            if (task && task->func) task->func(); // do the task
            if (_finishedOneJob)_finishedOneJob(task);
        }
    }

    // 从 Task Queue 中取出队首的 task
    Job* takeOneTask()
    {
        int len_old = _len.fetch_sub(1);
        // 非空时，取出一个队首 task
        if (len_old > 0)
        {
            return _queue.pop();
        }
        // 队列为空时，等待 Main Thread 推送新的 task 后通知
        else if (_is_running)
        {
            _len.fetch_add(1);	// 恢复原长度
            std::unique_lock<std::mutex> lk(_mtx);
            _cond.wait(lk);
        }
        return nullptr;
    }
public:
    // disable copy and assign construct
    ThreadPool(const ThreadPool&) = delete;

    ThreadPool& operator=(const ThreadPool& other) = delete;

private:
    std::mutex _mtx;
    std::condition_variable _cond;

    std::atomic_bool _is_running;
    std::atomic<int> _len;

    int _thread_num;
    std::vector<std::thread> _threads;

    LockFreeQueue<Job*> _queue;

    // 用于调用上层
    std::function<void(Job*)> _finishedOneJob;
};
