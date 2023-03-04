#pragma once

#include <atomic>

const unsigned int MAX_QUEUE_SIZE = 1024;

template<class T>
class LockFreeQueue 
{
public:

	T pop()
	{
		return mArr[mHead.fetch_add(1) % MAX_QUEUE_SIZE];
	};

	void push(T t)
	{
		mArr[mTail] = t;
		mTail = (mTail + 1) % MAX_QUEUE_SIZE;
	}

private:

	atomic<unsigned int> mHead = 0;
	unsigned int mTail = 0;
	T mArr[MAX_QUEUE_SIZE];
};