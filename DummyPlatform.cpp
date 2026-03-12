#include <chrono>
#include <cstdint>

extern "C" {

uint64_t os_gettime_ns(void)
{
	auto now = std::chrono::steady_clock::now();
	return (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}
}
