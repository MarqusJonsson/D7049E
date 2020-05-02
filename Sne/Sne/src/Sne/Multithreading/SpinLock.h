#pragma once
#include <atomic>

class SpinLock
{
private:
	std::atomic_flag lck = ATOMIC_FLAG_INIT;
public:
	void lock();
	bool try_lock();
	void unlock();
};