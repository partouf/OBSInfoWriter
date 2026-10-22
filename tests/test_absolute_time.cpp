#include <catch2/catch_test_macros.hpp>
#include "InfoWriter.h"
#include "DummyUtils.h"
#include <ctime>
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

static std::string local_time(std::time_t when)
{
	char buffer[64];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&when));
	return buffer;
}

static uint64_t seconds(uint64_t s)
{
	return s * 1000000000ull;
}

// The "@" times of events are clock times. They used to be calculated as the start time plus the recording time,
// which excludes pauses, so every event after a pause was logged too early by the length of the pause.

TEST_CASE("Event clock times after a pause are the actual time", "[absolutetime]")
{
	auto dir = std::filesystem::temp_directory_path() / "test_infowriter_absolutetime";
	std::filesystem::remove_all(dir);
	std::filesystem::create_directories(dir);
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	auto *settings = writer.GetSettings();
	settings->SetFilename(logfile.string());
	settings->SetFormat("%d:%02d:%02d");
	settings->SetShouldLogHotkeySpecifics(true);
	settings->SetShouldLogAbsoluteTime(true);
	settings->SetHotkeyText(1, "MARKER");

	// only OBS's clock is frozen, the wall clock keeps running normally; an hour long pause on OBS's clock must
	// not move the logged clock times
	const uint64_t start_ns = 1000000000000ull;
	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(10);
	writer.MarkPauseStart(imtRecordingPauseStart);
	dummy_time_ns = start_ns + seconds(3610);
	writer.MarkPauseResume(imtRecordingPauseResume);
	dummy_time_ns = start_ns + seconds(3620);
	auto before = std::time(nullptr);
	writer.WriteInfo((InfoHotkey)1);
	writer.WriteSceneChange("AfterPause");
	auto after = std::time(nullptr);
	writer.MarkStop(imtRecording);
	dummy_time_ns = std::nullopt;

	auto contents = read_file(logfile);
	bool hotkey_now = false;
	bool scene_now = false;
	for (auto t = before - 1; t <= after + 1; t++) {
		hotkey_now |= contents.find("HOTKEY:MARKER @ " + local_time(t)) != std::string::npos;
		scene_now |= contents.find("EVENT:SCENE CHANGED TO AfterPause @ " + local_time(t)) != std::string::npos;
	}
	CHECK(hotkey_now);
	CHECK(scene_now);

	std::filesystem::remove_all(dir);
}
