#include "InfoWriterObsUtils.h"
#include "DummyUtils.h"

std::optional<std::string> dummy_recording_path;
std::optional<std::string> dummy_last_recording;

std::optional<std::string> get_filename_from_recording_path([[maybe_unused]] const InfoWriterSettings &Settings)
{
	return dummy_recording_path;
}

std::optional<std::string> get_filename_from_last_recording([[maybe_unused]] const InfoWriterSettings &Settings)
{
	return dummy_last_recording;
}
