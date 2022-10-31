#pragma once

#include "InfoWriterSettings.h"
#include <cstdint>
#include <Groundfloor/Molecules/String.h>
#include "OutputFormat.h"
#include <memory>

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
	int64_t StartRecordTime;
	int64_t StartStreamTime;
	int64_t PausedTotalTime;
	int64_t PausedStartTime;

	InfoMediaType lastInfoMediaType;

	InfoWriterSettings Settings;
	bool StreamStarted;
	bool RecordStarted;
	bool Paused;

	std::string CurrentFilename;

	std::unique_ptr<IOutputFormat> output;

	int64_t getPausedTime(const int64_t currentTime) const;
	void InitCurrentFilename(int64_t timestamp);
	std::string SecsToHMSString(const int64_t totalseconds) const;

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

	std::string NowTimeStamp() const;

	InfoWriterSettings *GetSettings();
};
