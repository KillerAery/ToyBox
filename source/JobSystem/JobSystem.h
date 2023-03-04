#pragma once

#include <vector>
#include <atomic>
#include <functional>
#include "Job.h"
#include "ThreadPool.h"

using namespace std;

class JobSystem {

public:

	// Ӧ���� CPU ��������num���������̳߳�
	JobSystem(int num);

	~JobSystem();

	// ÿ֡��Ҫ��ʼ��
	void Init();

	// �ύ Job�������ȿ��ɣ�
	int SumbitJob(const std::function<void()>& func, std::list<int>&& parents = {});

	// ͬʱ������������ Job
	void StartJobs();

	// �ȴ����� Job ���
	void WaitJobs();

private:

	void FinishedOneJob(Job* job);

private:

	ThreadPool mThreadPool;			// �̳߳�
	std::vector<Job> mJobs;			// Jobs
	atomic<int> mCurrentJobsNum;	// ��ǰʣ�� Job ����

	std::mutex mMtx;
	condition_variable mWait;
};