#pragma once

#include <vector>
#include <atomic>
#include <functional>
#include "Job.h"
#include "ThreadPool.h"

using namespace std;

class JobSystem {

public:

	// 应根据 CPU 核心数（num）来创建线程池
	JobSystem(int num);

	~JobSystem();

	// 每帧都要初始化
	void Init();

	// 提交 Job（但不先开干）
	int SumbitJob(const std::function<void()>& func, std::list<int>&& parents = {});

	// 同时触发开干所有 Job
	void StartJobs();

	// 等待所有 Job 完成
	void WaitJobs();

private:

	void FinishedOneJob(Job* job);

private:

	ThreadPool mThreadPool;			// 线程池
	std::vector<Job> mJobs;			// Jobs
	atomic<int> mCurrentJobsNum;	// 当前剩余 Job 数量

	std::mutex mMtx;
	condition_variable mWait;
};