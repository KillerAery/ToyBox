#pragma once

#include "JobSystem.h"
#include <vector>
#include <atomic>
#include "Job.h"
#include "ThreadPool.h"

using namespace std;

// 应根据 CPU 核心数（num）来创建线程池

JobSystem::JobSystem(int num) :
	mThreadPool(num),
	mCurrentJobsNum(0)
{
	mThreadPool.setFinishFunc([this](Job* job) {this->FinishedOneJob(job); });
	mThreadPool.start();
}

JobSystem::~JobSystem() {
	mThreadPool.stop();
}

// 每帧都要初始化

void JobSystem::Init()
{
	mJobs.clear();
	mCurrentJobsNum.store(0);
	cout << "本帧的 Jobsystem 已初始化完成" << endl;
}

// 提交 Job（但不先开干）

int JobSystem::SumbitJob(const std::function<void()>& func, std::list<int>&& parents)
{
	size_t id = mJobs.size();
	mJobs.emplace_back(func);
	for (int parent : parents)
	{
		mJobs[parent].mChildren.emplace_back(id);
		// 当前任务依赖数+1
		mJobs[id].mDependece++;
	}
	return id;
}

// 同时触发开干所有 Job

void JobSystem::StartJobs()
{
	cout << "本帧的 Jobsystem 已开始同时触发" << endl;
	mCurrentJobsNum.store(mJobs.size());
	for (auto& job : mJobs)
	{
		if (job.mDependece == 0)
			mThreadPool.appendTask(&job);
	}
}

// 等待所有 Job 完成

void JobSystem::WaitJobs()
{
	std::unique_lock<std::mutex> lck(mMtx);
	mWait.wait(lck, [this]() {return mCurrentJobsNum == 0; });
	cout << "本帧的 Jobsystem 已完成所有 Job" << endl << endl;
}

void JobSystem::FinishedOneJob(Job* job)
{
	if (!job)return;
	for (auto child : job->mChildren)
	{
		// 对每个 child Job 的依赖数减去一
		if (mJobs[child].mDependece.fetch_sub(1) == 1)
		{
			mThreadPool.appendTask(&mJobs[child]);
		}
	}

	mCurrentJobsNum.fetch_sub(1);
	mWait.notify_one();
}
