#pragma once
#include <list>
#include <atomic>
#include <functional>

using namespace std;

struct Job
{
	Job() :
		mDependece(0)
	{}

	Job(const std::function<void()> & _func) :
		mFunc(_func),
		mDependece(0)
	{}

	Job(Job&& job) noexcept :
		mFunc(std::move(job.mFunc)),
		mChildren(std::move(job.mChildren)),
		mDependece(job.mDependece.load())
	{}

	std::function<void()> mFunc;	// 作业函数
	std::list<int> mChildren;		// 子作业
	atomic<int> mDependece;			// 依赖计数
};