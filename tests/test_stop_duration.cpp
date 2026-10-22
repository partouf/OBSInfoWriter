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

static void configure(InfoWriter &writer, const std::filesystem::path &logfile, const std::string &format)
{
	auto *settings = writer.GetSettings();
	settings->SetFilename(logfile.string());
	settings->SetFormat(format);
	settings->SetShouldLogStreaming(true);
}

static const uint64_t start_ns = 1000000000000ull;

static uint64_t seconds(uint64_t s)
{
	return s * 1000000000ull;
}

// Issue #80: the stop line should state how long the recording or stream lasted.

TEST_CASE("Stop recording line states the recording duration", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_record");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(18 * 60 + 30);
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	CHECK(contents.find("EVENT:STOP RECORDING @ ") != std::string::npos);
	CHECK(contents.find("Record Time Marker Reset to 0 Duration 0:18:30") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Recording duration excludes paused time", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_paused");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(10);
	writer.MarkPauseStart(imtRecordingPauseStart);
	dummy_time_ns = start_ns + seconds(15);
	writer.MarkPauseResume(imtRecordingPauseResume);
	dummy_time_ns = start_ns + seconds(25);
	writer.MarkStop(imtRecording);

	CHECK(read_file(logfile).find("Record Time Marker Reset to 0 Duration 0:00:20") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Recording stopped while paused excludes the pause", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_stoppaused");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(10);
	writer.MarkPauseStart(imtRecordingPauseStart);
	dummy_time_ns = start_ns + seconds(40);
	writer.MarkStop(imtRecording);

	CHECK(read_file(logfile).find("Record Time Marker Reset to 0 Duration 0:00:10") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Stop stream line states the stream duration", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_stream");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtStream);
	dummy_time_ns = start_ns + seconds(65);
	writer.MarkStop(imtStream);

	auto contents = read_file(logfile);
	CHECK(contents.find("EVENT:STOP STREAM @ ") != std::string::npos);
	CHECK(contents.find("Stream Time Marker Reset to 0 Duration 0:01:05") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Stream and recording durations are tracked separately", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_overlap");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtStream);
	dummy_time_ns = start_ns + seconds(60);
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(90);
	writer.MarkStop(imtRecording);
	dummy_time_ns = start_ns + seconds(120);
	writer.MarkStop(imtStream);

	auto contents = read_file(logfile);
	CHECK(contents.find("Record Time Marker Reset to 0 Duration 0:00:30") != std::string::npos);
	CHECK(contents.find("Stream Time Marker Reset to 0 Duration 0:02:00") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Duration uses the configured format including milliseconds", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_millis");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d.%03d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(83) + 456000000ull;
	writer.MarkStop(imtRecording);

	CHECK(read_file(logfile).find("Record Time Marker Reset to 0 Duration 0:01:23.456") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}

TEST_CASE("Stop line after a split states the last segment's duration", "[duration][issue80][split]")
{
	auto dir = make_test_dir("test_infowriter_duration_split");
	auto logfile = dir / "log.txt";

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	dummy_time_ns = start_ns;
	writer.MarkStart(imtRecording);
	dummy_time_ns = start_ns + seconds(67);
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	dummy_time_ns = start_ns + seconds(76);
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	CHECK(contents.find("0:01:07 Record Time Marker") != std::string::npos);
	CHECK(contents.find("Record Time Marker Reset to 0 Duration 0:00:09") != std::string::npos);

	dummy_time_ns = std::nullopt;
	std::filesystem::remove_all(dir);
}
