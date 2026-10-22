#include "DummyUtils.h"
#include <chrono>
#include <cstdint>

std::optional<uint64_t> dummy_time_ns;

extern "C" {

uint64_t os_gettime_ns(void)
{
	if (dummy_time_ns)
		return dummy_time_ns.value();

	auto now = std::chrono::steady_clock::now();
	return (uint64_t)std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}
}
