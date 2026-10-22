#include <catch2/catch_test_macros.hpp>
#include "InfoWriter.h"
#include "DummyUtils.h"
#include <filesystem>
#include <fstream>
#include <sstream>

static std::string read_file(const std::filesystem::path &path)
{
	std::ifstream file(path);
	std::stringstream contents;
	contents << file.rdbuf();
	return contents.str();
}

// Issue #95: when the log is renamed to match the video after recording stops, the output format
// must write to the renamed file while streaming continues, not recreate the old file.

TEST_CASE("Writes after recording stops go to the renamed file while still streaming", "[sync][issue95]")
{
	auto dir = std::filesystem::temp_directory_path() / "test_infowriter_issue95";
	std::filesystem::remove_all(dir);
	std::filesystem::create_directories(dir);

	auto fallback = dir / "fallback.txt";
	auto video_log = dir / "video.log";

	dummy_recording_path = std::nullopt;
	dummy_last_recording = video_log.string();

	InfoWriter writer;
	auto *settings = writer.GetSettings();
	settings->SetFilename(fallback.string());
	settings->SetFormat("%d:%02d:%02d");
	settings->SetShouldSyncNameAndPathWithVideo(true);

	writer.MarkStart(imtStream);
	writer.MarkStart(imtRecording);
	writer.MarkStop(imtRecording);

	REQUIRE(std::filesystem::exists(video_log));
	REQUIRE_FALSE(std::filesystem::exists(fallback));

	writer.WriteInfo("AfterRecording");
	writer.MarkStop(imtStream);

	CHECK_FALSE(std::filesystem::exists(fallback));

	auto contents = read_file(video_log);
	CHECK(contents.find("EVENT:STOP RECORDING") != std::string::npos);
	CHECK(contents.find("AfterRecording") != std::string::npos);
	CHECK(contents.find("EVENT:STOP STREAM") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

// When streaming starts before recording with sync enabled, there is no video to name the log after yet. The stream
// must not write into a previous recording's log; its lines go to the logfile, and each recording gets its own log.

static std::filesystem::path make_test_dir(const std::string &name)
{
	auto dir = std::filesystem::temp_directory_path() / name;
	std::filesystem::remove_all(dir);
	std::filesystem::create_directories(dir);
	return dir;
}

static void configure_sync(InfoWriter &writer, const std::filesystem::path &fallback)
{
	auto *settings = writer.GetSettings();
	settings->SetFilename(fallback.string());
	settings->SetFormat("%d:%02d:%02d");
	settings->SetShouldSyncNameAndPathWithVideo(true);
	settings->SetShouldLogStreaming(true);
}

static size_t count_occurrences(const std::string &haystack, const std::string &needle)
{
	size_t count = 0;
	for (auto pos = haystack.find(needle); pos != std::string::npos; pos = haystack.find(needle, pos + 1))
		count++;
	return count;
}

static void write_file(const std::filesystem::path &path, const std::string &contents)
{
	std::ofstream file(path);
	file << contents;
}

TEST_CASE("Stream started before recording keeps its lines in the logfile", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_streamfirst");
	auto fallback = dir / "fallback.txt";
	auto previous_log = dir / "previous.txt";
	auto video_log = dir / "video.txt";

	write_file(fallback, "EARLIER SESSION\n");
	write_file(previous_log, "PREVIOUS");

	// OBS still reports the previous recording's path while not recording
	dummy_recording_path = previous_log.string();
	dummy_last_recording = previous_log.string();

	InfoWriter writer;
	configure_sync(writer, fallback);

	writer.MarkStart(imtStream);
	writer.WriteInfo("BeforeRecording");

	dummy_recording_path = video_log.string();
	dummy_last_recording = video_log.string();

	writer.MarkStart(imtRecording);
	writer.WriteInfo("DuringRecording");
	writer.MarkStop(imtRecording);
	writer.MarkStop(imtStream);

	CHECK(read_file(previous_log) == "PREVIOUS");

	auto stream = read_file(fallback);
	CHECK(stream.find("EARLIER SESSION") == 0);
	CHECK(stream.find("EVENT:START STREAM") != std::string::npos);
	CHECK(stream.find("BeforeRecording") != std::string::npos);
	CHECK(stream.find("DuringRecording") == std::string::npos);

	auto video = read_file(video_log);
	CHECK(video.find("EARLIER SESSION") == std::string::npos);
	CHECK(video.find("EVENT:START STREAM") == std::string::npos);
	CHECK(video.find("BeforeRecording") == std::string::npos);
	CHECK(video.find("EVENT:START RECORDING") != std::string::npos);
	CHECK(video.find("DuringRecording") != std::string::npos);
	CHECK(video.find("EVENT:STOP RECORDING") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Stream without recording writes to the logfile instead of a previous recording's log", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_streamonly");
	auto fallback = dir / "fallback.txt";
	auto previous_log = dir / "previous.txt";

	write_file(previous_log, "PREVIOUS");

	dummy_recording_path = previous_log.string();
	dummy_last_recording = previous_log.string();

	InfoWriter writer;
	configure_sync(writer, fallback);

	writer.MarkStart(imtStream);
	writer.WriteInfo("StreamOnly");
	writer.MarkStop(imtStream);

	CHECK(read_file(previous_log) == "PREVIOUS");

	auto contents = read_file(fallback);
	CHECK(contents.find("EVENT:START STREAM") != std::string::npos);
	CHECK(contents.find("StreamOnly") != std::string::npos);
	CHECK(contents.find("EVENT:STOP STREAM") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Each recording during one stream gets its own log", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_streamtworecordings");
	auto fallback = dir / "fallback.txt";
	auto first_log = dir / "first.txt";
	auto second_log = dir / "second.txt";

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;

	InfoWriter writer;
	configure_sync(writer, fallback);

	writer.MarkStart(imtStream);

	dummy_recording_path = first_log.string();
	dummy_last_recording = first_log.string();
	writer.MarkStart(imtRecording);
	writer.WriteInfo("FirstRecording");
	writer.MarkStop(imtRecording);

	dummy_recording_path = second_log.string();
	dummy_last_recording = second_log.string();
	writer.MarkStart(imtRecording);
	writer.WriteInfo("SecondRecording");
	writer.MarkStop(imtRecording);

	writer.MarkStop(imtStream);

	CHECK(read_file(fallback).find("EVENT:START STREAM") != std::string::npos);

	auto first = read_file(first_log);
	CHECK(count_occurrences(first, "EVENT:START RECORDING") == 1);
	CHECK(first.find("FirstRecording") != std::string::npos);
	CHECK(first.find("SecondRecording") == std::string::npos);

	auto second = read_file(second_log);
	CHECK(count_occurrences(second, "EVENT:START RECORDING") == 1);
	CHECK(second.find("SecondRecording") != std::string::npos);
	CHECK(second.find("FirstRecording") == std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Recording log is written when the logfile folder doesn't exist", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_missingfolder");
	auto fallback = dir / "missing" / "log.txt";
	auto video_log = dir / "video.txt";

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;

	InfoWriter writer;
	configure_sync(writer, fallback);

	writer.MarkStart(imtStream);
	writer.WriteInfo("BeforeRecording");

	dummy_recording_path = video_log.string();
	dummy_last_recording = video_log.string();

	writer.MarkStart(imtRecording);
	writer.WriteInfo("DuringRecording");
	writer.MarkStop(imtRecording);
	writer.MarkStop(imtStream);

	CHECK_FALSE(std::filesystem::exists(fallback));

	auto video = read_file(video_log);
	CHECK(video.find("EVENT:START RECORDING") != std::string::npos);
	CHECK(video.find("DuringRecording") != std::string::npos);
	CHECK(video.find("EVENT:STOP RECORDING") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Subtitle log of a recording started during a stream only has the recording's cues", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_streamfirst_srt");
	auto fallback = dir / "fallback.srt";
	auto video_log = dir / "video.srt";

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;

	InfoWriter writer;
	configure_sync(writer, fallback);
	auto *settings = writer.GetSettings();
	settings->SetOutputFormat("srt");
	settings->SetShouldLogHotkeySpecifics(true);
	settings->SetHotkeyText(1, "BeforeRecording");
	settings->SetHotkeyText(2, "DuringRecording");

	writer.MarkStart(imtStream);
	writer.WriteInfo((InfoHotkey)1);

	dummy_recording_path = video_log.string();
	dummy_last_recording = video_log.string();

	writer.MarkStart(imtRecording);
	writer.WriteInfo((InfoHotkey)2);
	writer.MarkStop(imtRecording);
	writer.MarkStop(imtStream);

	auto video = read_file(video_log);
	CHECK(video.find("1") == 0);
	CHECK(count_occurrences(video, "START @") == 1);
	CHECK(video.find("BeforeRecording") == std::string::npos);
	CHECK(video.find("DuringRecording") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Stream started during a recording keeps writing to the recording's log", "[sync][streamfirst]")
{
	auto dir = make_test_dir("test_infowriter_recordfirst");
	auto fallback = dir / "fallback.txt";
	auto video_log = dir / "video.txt";

	dummy_recording_path = video_log.string();
	dummy_last_recording = video_log.string();

	InfoWriter writer;
	configure_sync(writer, fallback);

	writer.MarkStart(imtRecording);
	writer.MarkStart(imtStream);
	writer.MarkStop(imtStream);
	writer.MarkStop(imtRecording);

	CHECK_FALSE(std::filesystem::exists(fallback));

	auto contents = read_file(video_log);
	CHECK(contents.find("EVENT:START RECORDING") != std::string::npos);
	CHECK(contents.find("EVENT:START STREAM") != std::string::npos);
	CHECK(contents.find("EVENT:STOP STREAM") != std::string::npos);
	CHECK(contents.find("EVENT:STOP RECORDING") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}
