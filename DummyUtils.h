#pragma once

#include <cstdint>
#include <optional>
#include <string>

// Values returned by the dummy get_filename_from_* functions, so tests can simulate OBS recording paths
extern std::optional<std::string> dummy_recording_path;
extern std::optional<std::string> dummy_last_recording;

// When set, the dummy os_gettime_ns returns this instead of the real clock, so tests can control elapsed time
extern std::optional<uint64_t> dummy_time_ns;
