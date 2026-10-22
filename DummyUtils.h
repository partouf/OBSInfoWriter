#pragma once

#include <optional>
#include <string>

// Values returned by the dummy get_filename_from_* functions, so tests can simulate OBS recording paths
extern std::optional<std::string> dummy_recording_path;
extern std::optional<std::string> dummy_last_recording;
