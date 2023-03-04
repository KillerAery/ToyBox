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

        //�����߳�
        for (int i = 0; i < mNumThread; ++i)
            mThreads.emplace_back(std::thread(&ThreadPool::work, this));
    }

    void stop() {
        {
            //��Ϊ�е��߳̿��ܴ�������״̬,�����ڻ����߳�֮ǰ��֤�����߳̾���������״̬
            std::unique_lock<std::mutex> lk(mMtx);
            mIsRunning = false;
            mCV.notify_all();
        }

        for (auto& thread : mThreads) {
            if (thread.joinable())
                thread.join();
        }
    }

    //����������������
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

    //�����ӿ�
    void work() {
        while (mIsRunning)
        {
            // �� Task Queue ��ȡ��һ�� task
            Job* task = takeOneTask();
            if (task && task->mFunc) task->mFunc(); // do the task
            if (mFinishedOneJobDelegate)mFinishedOneJobDelegate(task);
        }
    }

    // �� Task Queue ��ȡ�����׵� task
    Job* takeOneTask()
    {
        int lenOld = mLen.fetch_sub(1);
        // �ǿ�ʱ��ȡ��һ������ task
        if (lenOld > 0)
        {
            return mJobQueue.pop();
        }
        // ����Ϊ��ʱ���ȴ� Main Thread �����µ� task ��֪ͨ
        else if (mIsRunning)
        {
            mLen.fetch_add(1);	// �ָ�ԭ����
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

    std::function<void(Job*)> mFinishedOneJobDelegate; // ���ڵ����ϲ�
};
