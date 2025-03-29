#include "InfoWriterObsUtils.h"

#include <obs-frontend-api.h>

std::optional<std::string> get_filename_from_recording_path(const InfoWriterSettings &Settings)
{
	obs_output_t *output = obs_frontend_get_recording_output();
	if (!output)
		return std::nullopt;

	obs_data_t *outputSettings = obs_output_get_settings(output);

	obs_data_item_t *item = obs_data_item_byname(outputSettings, "url");
	if (!item)
		item = obs_data_item_byname(outputSettings, "path");

	if (!item)
		return std::nullopt;

	std::string CurrentFilename = obs_data_item_get_string(item);
	size_t videoextensionstart = CurrentFilename.find_last_of('.') + 1;
	auto extension = Settings.GetAutomaticOutputExtension();
	CurrentFilename.replace(videoextensionstart, CurrentFilename.length(), extension);
	return CurrentFilename;
}
