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

static std::filesystem::path make_test_dir(const std::string &name)
{
	auto dir = std::filesystem::temp_directory_path() / name;
	std::filesystem::remove_all(dir);
	std::filesystem::create_directories(dir);
	return dir;
}

static void configure(InfoWriter &writer, const std::filesystem::path &logfile, bool sync)
{
	auto *settings = writer.GetSettings();
	settings->SetFilename(logfile.string());
	settings->SetFormat("%d:%02d:%02d");
	settings->SetAutomaticOutputExtension("txt");
	settings->SetShouldSyncNameAndPathWithVideo(sync);
	settings->SetShouldLogHotkeySpecifics(true);
	settings->SetHotkeyText(1, "FIRST");
	settings->SetHotkeyText(2, "SECOND");
}

// Issue #68: when OBS splits a recording into several files, each segment gets its own log.

TEST_CASE("Each recording segment gets its own synced log", "[split][issue68]")
{
	auto dir = make_test_dir("test_infowriter_split_sync");
	auto first_video = dir / "first.mkv";
	auto second_video = dir / "second.mkv";

	dummy_recording_path = (dir / "first.txt").string();
	dummy_last_recording = std::nullopt;

	InfoWriter writer;
	configure(writer, dir / "fallback.txt", true);

	writer.MarkStart(imtRecording);
	writer.WriteInfo((InfoHotkey)1);
	writer.HandleRecordingSplit(second_video.string());
	writer.WriteInfo((InfoHotkey)2);

	// OBS reports the last segment as the last recording
	dummy_last_recording = (dir / "second.txt").string();
	writer.MarkStop(imtRecording);

	CHECK_FALSE(std::filesystem::exists(dir / "fallback.txt"));

	auto first = read_file(dir / "first.txt");
	CHECK(first.find("EVENT:START RECORDING") != std::string::npos);
	CHECK(first.find("HOTKEY:FIRST") != std::string::npos);
	CHECK(first.find("EVENT:RECORDING SPLIT") != std::string::npos);
	CHECK(first.find("HOTKEY:SECOND") == std::string::npos);
	CHECK(first.find("EVENT:STOP RECORDING") == std::string::npos);

	auto second = read_file(dir / "second.txt");
	CHECK(second.find("EVENT:RECORDING SPLIT - NEW FILE") != std::string::npos);
	CHECK(second.find("HOTKEY:FIRST") == std::string::npos);
	CHECK(second.find("HOTKEY:SECOND") != std::string::npos);
	CHECK(second.find("EVENT:STOP RECORDING") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Split segment log name only replaces the extension of the file name", "[split][issue68]")
{
	auto dir = make_test_dir("test_infowriter_split.dotted");
	auto wrong_log = std::filesystem::temp_directory_path() / "test_infowriter_split.txt";
	std::filesystem::remove(wrong_log);

	dummy_recording_path = (dir / "first.txt").string();
	dummy_last_recording = std::nullopt;

	InfoWriter writer;
	configure(writer, dir / "fallback.txt", true);

	writer.MarkStart(imtRecording);
	writer.HandleRecordingSplit((dir / "second").string());
	writer.WriteInfo((InfoHotkey)2);
	writer.MarkStop(imtRecording);

	CHECK(std::filesystem::exists(dir / "second.txt"));
	CHECK_FALSE(std::filesystem::exists(wrong_log));

	std::filesystem::remove(wrong_log);
	dummy_recording_path = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Split without sync keeps writing to a fixed logfile", "[split][issue68]")
{
	auto dir = make_test_dir("test_infowriter_split_fixed");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, false);

	writer.MarkStart(imtRecording);
	writer.WriteInfo((InfoHotkey)1);
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	writer.WriteInfo((InfoHotkey)2);
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	CHECK(contents.find("HOTKEY:FIRST") != std::string::npos);
	CHECK(contents.find("EVENT:RECORDING SPLIT - NEW FILE") != std::string::npos);
	CHECK(contents.find("HOTKEY:SECOND") != std::string::npos);
	CHECK(contents.find("EVENT:STOP RECORDING") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Split is ignored when not recording", "[split][issue68]")
{
	auto dir = make_test_dir("test_infowriter_split_notrecording");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, false);

	writer.HandleRecordingSplit((dir / "second.mkv").string());
	CHECK_FALSE(std::filesystem::exists(logfile));

	writer.MarkStart(imtStream);
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	writer.MarkStop(imtStream);

	CHECK(read_file(logfile).find("EVENT:RECORDING SPLIT") == std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Stream lines keep going to the current segment's log after a split", "[split][issue68]")
{
	auto dir = make_test_dir("test_infowriter_split_streaming");

	dummy_recording_path = (dir / "first.txt").string();
	dummy_last_recording = (dir / "second.txt").string();

	InfoWriter writer;
	configure(writer, dir / "fallback.txt", true);

	writer.MarkStart(imtRecording);
	writer.MarkStart(imtStream);
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	writer.MarkStop(imtRecording);
	writer.WriteInfo((InfoHotkey)2);
	writer.MarkStop(imtStream);

	auto second = read_file(dir / "second.txt");
	CHECK(second.find("EVENT:STOP RECORDING") != std::string::npos);
	CHECK(second.find("HOTKEY:SECOND") != std::string::npos);

	dummy_recording_path = std::nullopt;
	dummy_last_recording = std::nullopt;
	std::filesystem::remove_all(dir);
}
