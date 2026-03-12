#pragma once

#include "InfoWriterSettings.h"
#include <cstdint>
#include <Groundfloor/Molecules/String.h>
#include "OutputFormat.h"
#include <memory>
#include <filesystem>

enum InfoMediaType {
	imtUnknown = 0,
	imtStream = 1,
	imtRecording = 2,
	imtRecordingPauseStart = 3,
	imtRecordingPauseResume = 4
};
typedef int8_t InfoHotkey;

class InfoWriter {
private:
	int64_t StartTime;

	uint64_t StartRecordTimeNs;
	uint64_t StartStreamTimeNs;
	uint64_t PausedTotalTimeNs;
	uint64_t PausedStartTimeNs;

	InfoMediaType lastInfoMediaType;

	InfoWriterSettings Settings;
	bool StreamStarted;
	bool RecordStarted;
	bool Paused;

	std::string CurrentFilename;

	std::unique_ptr<IOutputFormat> output;

	uint64_t getPausedTimeNs(const uint64_t currentTimeNs) const;
	void InitCurrentFilename();
	bool RenameCurrentFile(const std::string &newFilename);
	std::string MillisToHMSString(const int64_t totalmilliseconds) const;

public:
	InfoWriter();

	void MarkStart(const InfoMediaType AType);
	void MarkPauseStart(const InfoMediaType AType);
	void MarkPauseResume(const InfoMediaType AType);
	void WriteInfo(const std::string AExtraInfo) const;
	void WriteInfo(const InfoHotkey AHotkey) const;
	void WriteSceneChange(const std::string scenename) const;
	void MarkStop(const InfoMediaType AType);

	bool HasStarted() const;
	bool IsStreaming() const;
	bool IsRecording() const;

	std::string NowTimeStamp() const;

	InfoWriterSettings *GetSettings();
};
