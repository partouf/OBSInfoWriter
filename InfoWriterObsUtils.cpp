#include "InfoWriterObsUtils.h"
#include "FormatUtils.h"

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
	return ReplaceFileExtension(CurrentFilename, Settings.GetAutomaticOutputExtension());
}

std::optional<std::string> get_filename_from_last_recording(const InfoWriterSettings &Settings)
{
	char *path = obs_frontend_get_last_recording();
	if (!path)
		return std::nullopt;

	std::string filename = path;
	bfree(path);

	if (filename.empty())
		return std::nullopt;

	return ReplaceFileExtension(filename, Settings.GetAutomaticOutputExtension());
}
