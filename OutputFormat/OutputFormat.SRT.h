#pragma once

#include "../OutputFormat.h"
#include "../InfoWriter.h"
#include <string>

class OutputFormatSRT final : public IOutputFormat {
private:
	const InfoWriterSettings &settings;
	std::string currentFilename;
	int subtitleCounter;
	bool canAppend;
	void WriteGFStringToFile(const std::string filename, const std::string text) const;
	std::string MillisToHMSString(const int64_t totalmilliseconds) const;
	void WriteLines(const int64_t milliseconds, const std::string text);

public:
	OutputFormatSRT(const InfoWriterSettings &settings, const std::string filename);

	void Start() override;
	void Stop(const int64_t milliseconds) override;
	void HotkeyMarker(const int64_t milliseconds, const std::string text) override;
	void ScenechangeMarker(const int64_t milliseconds, const std::string scenename) override;
	void PausedMarker(const int64_t milliseconds) override;
	void ResumedMarker(const int64_t milliseconds, const int64_t pauselength_ms) override;
	void TextMarker(const std::string text) override;
};
