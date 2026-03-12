#pragma once

#include "../OutputFormat.h"
#include "../InfoWriterSettings.h"
#include <Groundfloor/Molecules/String.h>

class OutputFormatEDL final : public IOutputFormat {
private:
	int markercount;
	int64_t lastMarker;
	std::string lastMarkerText;
	const InfoWriterSettings &settings;
	std::string currentFilename;

	void WriteGFStringToFile(const Groundfloor::String &SData) const;
	std::string MillisToHMSString(const int64_t totalmilliseconds) const;
	void writeMarker(const int64_t start_ms, const int64_t stop_ms, const std::string text) const;

public:
	OutputFormatEDL(const InfoWriterSettings &settings, const std::string filename);

	void Start() override;
	void Stop(const int64_t milliseconds) override;

	void HotkeyMarker(const int64_t milliseconds, const std::string text) override;
	void ScenechangeMarker(const int64_t milliseconds, const std::string scenename) override;
	void PausedMarker(const int64_t milliseconds) override;
	void ResumedMarker(const int64_t milliseconds, const int64_t pauselength_ms) override;
	void TextMarker(const std::string text) override;
};
