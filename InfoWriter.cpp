#include "InfoWriter.h"

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Materials/Functions.h>
#include <cstdint>
#include <memory>

#include "OutputFormat/OutputFormat.Default.h"
#include "OutputFormat/OutputFormat.EDL.h"
#include "OutputFormat/OutputFormat.CSV.h"
#include "OutputFormat/OutputFormat.SRT.h"

#include "InfoWriterObsUtils.h"
#include "FormatUtils.h"

#include <util/platform.h>
#include <util/base.h>

const char *c_TimestampNotation = "%Y-%m-%d %H:%M:%S";

InfoWriter::InfoWriter() : Settings()
{
	lastInfoMediaType = imtUnknown;
	StartTime = 0;
	StartRecordTimeNs = 0;
	StartStreamTimeNs = 0;
	PausedTotalTimeNs = 0;
	PausedStartTimeNs = 0;
	StreamStarted = false;
	RecordStarted = false;
	Paused = false;
	SceneIsChanging = false;
}

std::string InfoWriter::MillisToHMSString(const int64_t totalmilliseconds) const
{
	return FormatMillisToHMS(Settings.GetFormat(), totalmilliseconds);
}

uint64_t InfoWriter::getPausedTimeNs(const uint64_t currentTimeNs) const
{
	auto PausedTmpTimeNs = PausedTotalTimeNs;

	if (Paused) {
		PausedTmpTimeNs = PausedTotalTimeNs + (currentTimeNs - PausedStartTimeNs);
	}

	return PausedTmpTimeNs;
}

void InfoWriter::WriteInfo(const std::string AExtraInfo) const
{
	if (output == nullptr)
		return;

	char crlf[] = GFNATIVENEXTLINE;

	std::string Info;
	auto currentTimeNs = os_gettime_ns();
	auto tmpTime = MillisToHMSString(0);

	auto PausedTmpTimeNs = getPausedTimeNs(currentTimeNs);

	if (RecordStarted) {
		int64_t elapsedMs = (int64_t)((currentTimeNs - StartRecordTimeNs - PausedTmpTimeNs) / 1000000);
		tmpTime = MillisToHMSString(elapsedMs);
	}

	std::string record_info = tmpTime;
	tmpTime = MillisToHMSString(0);
	if (StreamStarted) {
		int64_t elapsedMs = (int64_t)((currentTimeNs - StartStreamTimeNs) / 1000000);
		tmpTime = MillisToHMSString(elapsedMs);
	}
	auto stream_info = tmpTime;

	record_info += " Record Time Marker";
	stream_info += " Stream Time Marker";

	if (!RecordStarted)
		record_info += " (not recording)";
	if (Paused)
		record_info += " (recording is paused)";
	if (!StreamStarted)
		stream_info += " (not streaming)";

	record_info += crlf;
	stream_info += crlf;

	if (AExtraInfo.length() != 0) {
		Info = "";
		Info += AExtraInfo;
		Info += crlf;
	}

	Info += record_info;
	if (this->Settings.GetShouldLogStreaming())
		Info += stream_info;

	output->TextMarker(Info);
}

void InfoWriter::WriteInfo(const InfoHotkey AHotkey) const
{
	if (output == nullptr)
		return;

	auto NowNs = os_gettime_ns();
	auto hotkey_text = Settings.GetHotkeyText(AHotkey);

	if (lastInfoMediaType == imtStream) {
		if (StartStreamTimeNs == 0)
			return;

		int64_t elapsedMs = (int64_t)((NowNs - StartStreamTimeNs) / 1000000);
		output->HotkeyMarker(elapsedMs, hotkey_text);
	} else {
		if (StartRecordTimeNs == 0)
			return;

		int64_t elapsedMs = (int64_t)((NowNs - StartRecordTimeNs - getPausedTimeNs(NowNs)) / 1000000);
		output->HotkeyMarker(elapsedMs, hotkey_text);
	}

	this->WriteInfo("");
}

void InfoWriter::SetSceneIsChanging(bool yesno)
{
	this->SceneIsChanging = yesno;
}

void InfoWriter::WriteSceneChange(const std::string scenename) const
{
	if (output == nullptr)
		return;

	auto NowNs = os_gettime_ns();

	if (lastInfoMediaType == imtStream) {
		if (StartStreamTimeNs == 0)
			return;
		int64_t elapsedMs = (int64_t)((NowNs - StartStreamTimeNs) / 1000000);
		if (scenename == "") {
			output->ScenechangeMarker(elapsedMs, "UNKNO");
		} else {
			output->ScenechangeMarker(elapsedMs, scenename);
		}
	} else {
		if (StartRecordTimeNs == 0)
			return;
		int64_t elapsedMs = (int64_t)((NowNs - StartRecordTimeNs - getPausedTimeNs(NowNs)) / 1000000);
		if (scenename == "") {
			output->ScenechangeMarker(elapsedMs, "UNKNO");
		} else {
			output->ScenechangeMarker(elapsedMs, scenename);
		}
	}

	this->WriteInfo("");
}

void InfoWriter::InitCurrentFilename()
{
	bool currentname_set = false;

	if (Settings.GetShouldSyncNameAndPathWithVideo()) {
		auto filename = get_filename_from_recording_path(Settings);
		if (filename) {
			CurrentFilename = filename.value();
			currentname_set = true;
		}
	}

	if (!currentname_set) {
		CurrentFilename = Settings.GetFilename();
		if (CurrentFilename.find('%') != 0) {
			try {
				auto filename = Groundfloor::TimestampToStr(CurrentFilename.c_str(), StartTime);
				CurrentFilename = filename->getValue();
				delete filename;
			} catch (const std::runtime_error &e) {
				blog(LOG_ERROR, "[OBSInfoWriter] Invalid filename format: %s", e.what());
				CurrentFilename = Settings.GetFilename();
			}
		}
	}
}

bool InfoWriter::RenameCurrentFile(const std::string &newFilename)
{
	if (CurrentFilename.empty() || newFilename.empty())
		return false;

	if (CurrentFilename == newFilename)
		return false;

	std::error_code ec;
	std::filesystem::rename(CurrentFilename, newFilename, ec);
	if (ec)
		return false;

	CurrentFilename = newFilename;
	return true;
}

void InfoWriter::MarkStart(InfoMediaType AType)
{
	StartTime = Groundfloor::GetTimestamp();

	InitCurrentFilename();

	auto outputformat = Settings.GetOutputFormat();
	if (outputformat == "csv") {
		output = std::make_unique<OutputFormatCSV>(Settings, CurrentFilename);
	} else if (outputformat == "edl") {
		output = std::make_unique<OutputFormatEDL>(Settings, CurrentFilename);
	} else if (outputformat == "srt") {
		output = std::make_unique<OutputFormatSRT>(Settings, CurrentFilename);
	} else {
		output = std::make_unique<OutputFormatDefault>(Settings, CurrentFilename);
	}

	output->Start();

	auto MarkStr =
		std::unique_ptr<Groundfloor::String>{Groundfloor::TimestampToStr(c_TimestampNotation, StartTime)};

	lastInfoMediaType = AType;

	switch (AType) {
	case imtStream:
		StartStreamTimeNs = os_gettime_ns();
		StreamStarted = true;
		if (this->Settings.GetShouldLogStreaming()) {
			MarkStr->prepend_ansi("EVENT:START STREAM @ ");
		}
		output->TextMarker(MarkStr->getValue());
		break;
	case imtUnknown:
		MarkStr->prepend_ansi(" (WARNING:Unsure how we STARTED) ");
		break;
	case imtRecording:
		MarkStr->prepend_ansi("EVENT:START RECORDING @ ");
		StartRecordTimeNs = os_gettime_ns();
		RecordStarted = true;
		Paused = false;
		output->TextMarker(MarkStr->getValue());
		break;
	case imtRecordingPauseStart:
		break;
	case imtRecordingPauseResume:
		break;
	}

	this->WriteInfo("");
}

void InfoWriter::MarkStop(InfoMediaType AType)
{
	if (output == nullptr)
		return;

	auto NowNs = os_gettime_ns();
	auto Now = Groundfloor::GetTimestamp();
	auto MarkStr = std::unique_ptr<Groundfloor::String>{Groundfloor::TimestampToStr(c_TimestampNotation, Now)};

	switch (AType) {
	case imtStream:
		if (StartStreamTimeNs == 0)
			return;
		output->Stop((int64_t)((NowNs - StartStreamTimeNs) / 1000000));
		MarkStr->prepend_ansi("EVENT:STOP STREAM @ ");
		MarkStr->append(" Stream Time Marker Reset to 0");
		StreamStarted = false;
		StartStreamTimeNs = 0;
		break;
	case imtUnknown:
	case imtRecording:
		if (StartRecordTimeNs == 0)
			return;
		output->Stop((int64_t)((NowNs - StartRecordTimeNs - getPausedTimeNs(NowNs)) / 1000000));
		MarkStr->prepend_ansi("EVENT:STOP RECORDING @ ");
		MarkStr->append(" Record Time Marker Reset to 0");
		StartRecordTimeNs = 0;
		PausedTotalTimeNs = 0;
		Paused = false;
		RecordStarted = false;
		break;
	case imtRecordingPauseStart:
		break;
	case imtRecordingPauseResume:
		break;
	}

	WriteInfo(MarkStr->getValue());

	if (AType == imtRecording && Settings.GetShouldSyncNameAndPathWithVideo()) {
		auto lastRecording = get_filename_from_last_recording(Settings);
		if (lastRecording)
			RenameCurrentFile(lastRecording.value());
	}

	if (!IsStreaming() && !IsRecording()) {
		output = nullptr;
	}
}

void InfoWriter::MarkPauseStart([[maybe_unused]] const InfoMediaType AType)
{
	if (output == nullptr)
		return;

	Paused = true;
	PausedStartTimeNs = os_gettime_ns();

	int64_t elapsedMs = (int64_t)((PausedStartTimeNs - StartRecordTimeNs - PausedTotalTimeNs) / 1000000);
	output->PausedMarker(elapsedMs);
	this->WriteInfo("");
}

void InfoWriter::MarkPauseResume([[maybe_unused]] const InfoMediaType AType)
{
	if (output == nullptr)
		return;

	Paused = false;
	auto CurrentTimeNs = os_gettime_ns();
	auto pauseDurationNs = CurrentTimeNs - PausedStartTimeNs;
	PausedTotalTimeNs += pauseDurationNs;

	int64_t elapsedMs = (int64_t)((CurrentTimeNs - StartRecordTimeNs - PausedTotalTimeNs) / 1000000);
	int64_t pauseDurationMs = (int64_t)(pauseDurationNs / 1000000);
	output->ResumedMarker(elapsedMs, pauseDurationMs);
	this->WriteInfo("");
}

bool InfoWriter::HasStarted() const
{
	if (StreamStarted)
		return true;
	if (RecordStarted)
		return true;
	return false;
}

bool InfoWriter::IsStreaming() const
{
	return StreamStarted;
}

bool InfoWriter::IsRecording() const
{
	return RecordStarted;
}

bool InfoWriter::IsChangingScene() const
{
	return SceneIsChanging;
}

std::string InfoWriter::NowTimeStamp() const
{
	auto NowStr = Groundfloor::TimestampToStr(c_TimestampNotation, StartTime);
	return NowStr->getValue();
}

InfoWriterSettings *InfoWriter::GetSettings()
{
	return &Settings;
}
