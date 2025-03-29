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
	std::string SecsToHMSString(const int64_t totalseconds) const;
	void writeMarker(const int64_t start, const int64_t stop, const std::string text) const;

public:
	OutputFormatEDL(const InfoWriterSettings &settings, const std::string filename);

	void Start() override;
	void Stop(const int64_t timestamp) override;

	void HotkeyMarker(const int64_t timestamp, const std::string text) override;
	void ScenechangeMarker(const int64_t timestamp, const std::string scenename) override;
	void PausedMarker(const int64_t timestamp) override;
	void ResumedMarker(const int64_t timestamp, const int64_t pauselength) override;
	void TextMarker(const std::string text) override;
};
