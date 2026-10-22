#include <catch2/catch_test_macros.hpp>
#include "InfoWriter.h"
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

static void configure(InfoWriter &writer, const std::filesystem::path &logfile)
{
	auto *settings = writer.GetSettings();
	settings->SetFilename(logfile.string());
	settings->SetFormat("%d:%02d:%02d");
	settings->SetShouldLogHotkeySpecifics(true);
	settings->SetHotkeyText(1, "MARKER");
}

// Issue #98: when streaming and recording overlap, events after one of them stops must still be
// logged against the one that is still running.

TEST_CASE("Scene changes and hotkeys are logged while streaming after recording stops", "[overlap][issue98]")
{
	auto dir = make_test_dir("test_infowriter_issue98_stream");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile);

	writer.MarkStart(imtStream);
	writer.MarkStart(imtRecording);
	writer.MarkStop(imtRecording);

	writer.WriteSceneChange("AfterRecording");
	writer.WriteInfo((InfoHotkey)1);
	writer.MarkStop(imtStream);

	auto contents = read_file(logfile);
	CHECK(contents.find("EVENT:SCENE CHANGED TO AfterRecording") != std::string::npos);
	CHECK(contents.find("HOTKEY:MARKER") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Scene changes and hotkeys are logged while recording after streaming stops", "[overlap][issue98]")
{
	auto dir = make_test_dir("test_infowriter_issue98_record");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile);

	writer.MarkStart(imtRecording);
	writer.MarkStart(imtStream);
	writer.MarkStop(imtStream);

	writer.WriteSceneChange("AfterStreaming");
	writer.WriteInfo((InfoHotkey)1);
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	CHECK(contents.find("EVENT:SCENE CHANGED TO AfterStreaming") != std::string::npos);
	CHECK(contents.find("HOTKEY:MARKER") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Scene changes and hotkeys are not logged after everything stops", "[overlap][issue98]")
{
	auto dir = make_test_dir("test_infowriter_issue98_stopped");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile);

	writer.MarkStart(imtStream);
	writer.MarkStart(imtRecording);
	writer.MarkStop(imtRecording);
	writer.MarkStop(imtStream);

	writer.WriteSceneChange("AfterEverything");
	writer.WriteInfo((InfoHotkey)1);

	auto contents = read_file(logfile);
	CHECK(contents.find("AfterEverything") == std::string::npos);
	CHECK(contents.find("HOTKEY:MARKER") == std::string::npos);

	std::filesystem::remove_all(dir);
}
