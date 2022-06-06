#pragma once

#include <atomic>

const unsigned int MAX_QUEUE_SIZE = 1024;

template<class T>
class LockFreeQueue 
{
public:
	T pop()
	{
		return arr[head.fetch_add(1) % MAX_QUEUE_SIZE];
	};

	void push(T t)
	{
		arr[tail] = t;
		tail = (tail + 1) % MAX_QUEUE_SIZE;
	}
private:
	atomic<unsigned int> head = 0;
	unsigned int tail = 0;
	T arr[MAX_QUEUE_SIZE];
};