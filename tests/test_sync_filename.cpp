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
