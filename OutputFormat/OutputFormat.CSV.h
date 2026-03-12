#pragma once

#include "../OutputFormat.h"
#include "../InfoWriter.h"
#include <string>

class OutputFormatCSV final : public IOutputFormat {
private:
	const InfoWriterSettings &settings;
	std::string currentFilename;
	std::string MillisToHMSString(const int64_t totalmilliseconds) const;
	void WriteGFStringToFile(const std::string filename, const std::string text) const;
	void WriteCSVLine(const int64_t milliseconds, const std::string text) const;

public:
	OutputFormatCSV(const InfoWriterSettings &settings, const std::string filename);

	void Start() override;
	void Stop(const int64_t milliseconds) override;
	void HotkeyMarker(const int64_t milliseconds, const std::string text) override;
	void ScenechangeMarker(const int64_t milliseconds, const std::string scenename) override;
	void PausedMarker(const int64_t milliseconds) override;
	void ResumedMarker(const int64_t milliseconds, const int64_t pauselength_ms) override;
	void TextMarker(const std::string text) override;
};
