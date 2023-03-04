#pragma once

#include "JobSystem.h"
#include <vector>
#include <atomic>
#include "Job.h"
#include "ThreadPool.h"

using namespace std;

// Ӧ���� CPU ��������num���������̳߳�

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

// ÿ֡��Ҫ��ʼ��

void JobSystem::Init()
{
	mJobs.clear();
	mCurrentJobsNum.store(0);
	cout << "��֡�� Jobsystem �ѳ�ʼ�����" << endl;
}

// �ύ Job�������ȿ��ɣ�

int JobSystem::SumbitJob(const std::function<void()>& func, std::list<int>&& parents)
{
	size_t id = mJobs.size();
	mJobs.emplace_back(func);
	for (int parent : parents)
	{
		mJobs[parent].mChildren.emplace_back(id);
		// ��ǰ����������+1
		mJobs[id].mDependece++;
	}
	return id;
}

// ͬʱ������������ Job

void JobSystem::StartJobs()
{
	cout << "��֡�� Jobsystem �ѿ�ʼͬʱ����" << endl;
	mCurrentJobsNum.store(mJobs.size());
	for (auto& job : mJobs)
	{
		if (job.mDependece == 0)
			mThreadPool.appendTask(&job);
	}
}

// �ȴ����� Job ���

void JobSystem::WaitJobs()
{
	std::unique_lock<std::mutex> lck(mMtx);
	mWait.wait(lck, [this]() {return mCurrentJobsNum == 0; });
	cout << "��֡�� Jobsystem ��������� Job" << endl << endl;
}

void JobSystem::FinishedOneJob(Job* job)
{
	if (!job)return;
	for (auto child : job->mChildren)
	{
		// ��ÿ�� child Job ����������ȥһ
		if (mJobs[child].mDependece.fetch_sub(1) == 1)
		{
			mThreadPool.appendTask(&mJobs[child]);
		}
	}

	mCurrentJobsNum.fetch_sub(1);
	mWait.notify_one();
}
