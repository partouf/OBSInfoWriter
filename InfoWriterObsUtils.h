#pragma once

#include <optional>
#include <string>
#include "InfoWriterSettings.h"

std::optional<std::string> get_filename_from_recording_path(const InfoWriterSettings &Settings);
