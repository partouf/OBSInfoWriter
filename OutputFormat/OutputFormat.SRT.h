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
	void WriteGFStringToFile(const std::string filename,
				 const std::string text) const;
	std::string SecsToHMSString(const int64_t totalseconds) const;
	void WriteLines(const int64_t timestamp, const std::string text);

public:
	OutputFormatSRT(const InfoWriterSettings &settings,
			const std::string filename);

	void Start() override;
	void Stop(const int64_t timestamp) override;
	void HotkeyMarker(const int64_t timestamp,
			  const std::string text) override;
	void ScenechangeMarker(const int64_t timestamp,
			       const std::string scenename) override;
	void PausedMarker(const int64_t timestamp) override;
	void ResumedMarker(const int64_t timestamp,
			   const int64_t pauselength) override;
	void TextMarker(const std::string text) override;
};
