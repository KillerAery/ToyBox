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

        //�����߳�
        for (int i = 0; i < _thread_num; ++i)
            _threads.emplace_back(std::thread(&ThreadPool::work, this));
    }

    void stop() {
        {
            //��Ϊ�е��߳̿��ܴ�������״̬,�����ڻ����߳�֮ǰ��֤�����߳̾���������״̬
            std::unique_lock<std::mutex> lk(_mtx);
            _is_running = false;
            _cond.notify_all();
        }

        for (auto& thread : _threads) {
            if (thread.joinable())
                thread.join();
        }
    }

    //����������������
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
    //�����ӿ�
    void work() {
        while (_is_running)
        {
            // �� Task Queue ��ȡ��һ�� task
            Job* task = takeOneTask();
            if (task && task->func) task->func(); // do the task
            if (_finishedOneJob)_finishedOneJob(task);
        }
    }

    // �� Task Queue ��ȡ�����׵� task
    Job* takeOneTask()
    {
        int len_old = _len.fetch_sub(1);
        // �ǿ�ʱ��ȡ��һ������ task
        if (len_old > 0)
        {
            return _queue.pop();
        }
        // ����Ϊ��ʱ���ȴ� Main Thread �����µ� task ��֪ͨ
        else if (_is_running)
        {
            _len.fetch_add(1);	// �ָ�ԭ����
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

    // ���ڵ����ϲ�
    std::function<void(Job*)> _finishedOneJob;
};
