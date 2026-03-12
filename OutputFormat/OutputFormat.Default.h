#pragma once

#include "../OutputFormat.h"
#include "../InfoWriterSettings.h"
#include <Groundfloor/Molecules/String.h>

class OutputFormatDefault final : public IOutputFormat {
private:
	const InfoWriterSettings &settings;
	std::string currentFilename;
	int64_t startTime;

	std::string MillisToHMSString(const int64_t totalmilliseconds) const;
	void WriteToFile(const std::string Data) const;
	void WriteDblLineToFile(const std::string Data) const;
	void WriteGFStringToFile(const Groundfloor::String &SData) const;

public:
	OutputFormatDefault(const InfoWriterSettings &settings, const std::string filename);

	void Start() override;
	void Stop(const int64_t milliseconds) override;

	void HotkeyMarker(const int64_t milliseconds, const std::string text) override;
	void ScenechangeMarker(const int64_t milliseconds, const std::string scenename) override;
	void PausedMarker(const int64_t milliseconds) override;
	void ResumedMarker(const int64_t milliseconds, const int64_t pauselength_ms) override;

	void TextMarker(const std::string text) override;
};
