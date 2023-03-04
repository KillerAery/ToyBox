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

    explicit ThreadPool(int num) : mNumThread(num), mIsRunning(false), mLen(0) {}

    ~ThreadPool() {
        if (mIsRunning)
            stop();
    }

    void start() {
        mIsRunning = true;

        //开启线程
        for (int i = 0; i < mNumThread; ++i)
            mThreads.emplace_back(std::thread(&ThreadPool::work, this));
    }

    void stop() {
        {
            //因为有的线程可能处于阻塞状态,所以在回收线程之前保证所有线程均处于运行状态
            std::unique_lock<std::mutex> lk(mMtx);
            mIsRunning = false;
            mCV.notify_all();
        }

        for (auto& thread : mThreads) {
            if (thread.joinable())
                thread.join();
        }
    }

    //往任务队列添加任务
    void appendTask(Job* job) {
        if (mIsRunning) {
            mJobQueue.push(job);
            mLen.fetch_add(1);
            mCV.notify_one();
        }
    }

    template<class U>
    void setFinishFunc(U && func){
        mFinishedOneJobDelegate = std::forward<U>(func);
    }

private:

    //工作接口
    void work() {
        while (mIsRunning)
        {
            // 从 Task Queue 中取出一个 task
            Job* task = takeOneTask();
            if (task && task->mFunc) task->mFunc(); // do the task
            if (mFinishedOneJobDelegate)mFinishedOneJobDelegate(task);
        }
    }

    // 从 Task Queue 中取出队首的 task
    Job* takeOneTask()
    {
        int lenOld = mLen.fetch_sub(1);
        // 非空时，取出一个队首 task
        if (lenOld > 0)
        {
            return mJobQueue.pop();
        }
        // 队列为空时，等待 Main Thread 推送新的 task 后通知
        else if (mIsRunning)
        {
            mLen.fetch_add(1);	// 恢复原长度
            std::unique_lock<std::mutex> lk(mMtx);
            mCV.wait(lk);
        }
        return nullptr;
    }

public:

    ThreadPool(const ThreadPool&) = delete;

    ThreadPool& operator=(const ThreadPool& other) = delete;

private:

    std::mutex mMtx;
    std::condition_variable mCV;
    std::atomic_bool mIsRunning;
    std::atomic<int> mLen;

    int mNumThread;
    std::vector<std::thread> mThreads;

    LockFreeQueue<Job*> mJobQueue;

    std::function<void(Job*)> mFinishedOneJobDelegate; // 用于调用上层
};
