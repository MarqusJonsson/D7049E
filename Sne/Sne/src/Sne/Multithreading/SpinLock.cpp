#include "SpinLock.h"

void SpinLock::lock()
{
	while (!try_lock()) {}
}

bool SpinLock::try_lock()
{
	return !lck.test_and_set(std::memory_order_acquire);
}

void SpinLock::unlock()
{
	lck.clear(std::memory_order_release);
}
