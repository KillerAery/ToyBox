#pragma once
#include <list>
#include <atomic>
#include <functional>

using namespace std;

struct Job
{
	Job() :
		dependece(0)
	{}

	Job(const std::function<void()> & _func) :
		func(_func),
		dependece(0)
	{}

	Job(Job&& job) noexcept :
		func(std::move(job.func)),
		children(std::move(job.children)),
		dependece(job.dependece.load())
	{}

	std::function<void()> func;
	std::list<int> children;
	atomic<int> dependece;
};