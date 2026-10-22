#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
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

// returns the whole line containing the needle, without line endings
static std::string line_with(const std::string &contents, const std::string &needle)
{
	std::istringstream lines(contents);
	std::string line;
	while (std::getline(lines, line)) {
		if (!line.empty() && line.back() == '\r')
			line.pop_back();
		if (line.find(needle) != std::string::npos)
			return line;
	}
	return "";
}

static uint64_t seconds(uint64_t s)
{
	return s * 1000000000ull;
}

// Freezes the dummy OBS clock for the duration of a test, and always unfreezes it again
class FrozenClock {
public:
	static const uint64_t start_ns = 1000000000000ull;

	FrozenClock() { dummy_time_ns = start_ns; }
	~FrozenClock() { dummy_time_ns = std::nullopt; }

	void at(uint64_t ns) { dummy_time_ns = start_ns + ns; }
};

// Issue #80: the stop line should state how long the recording or stream lasted.

TEST_CASE("Stop recording line states the recording duration", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_record");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(18 * 60 + 30));
	writer.MarkStop(imtRecording);

	auto line = line_with(read_file(logfile), "EVENT:STOP RECORDING @ ");
	CHECK(line.size() > 0);
	CHECK(line.rfind(" Record Time Marker Reset to 0 Duration 0:18:30") ==
	      line.size() - std::string(" Record Time Marker Reset to 0 Duration 0:18:30").size());

	std::filesystem::remove_all(dir);
}

TEST_CASE("Recording duration excludes paused time", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_paused");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(10));
	writer.MarkPauseStart(imtRecordingPauseStart);
	clock.at(seconds(15));
	writer.MarkPauseResume(imtRecordingPauseResume);
	clock.at(seconds(25));
	writer.MarkStop(imtRecording);

	CHECK(line_with(read_file(logfile), "EVENT:STOP RECORDING").find("Reset to 0 Duration 0:00:20") !=
	      std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Recording stopped while paused excludes the pause", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_stoppaused");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(10));
	writer.MarkPauseStart(imtRecordingPauseStart);
	clock.at(seconds(40));
	writer.MarkStop(imtRecording);

	CHECK(line_with(read_file(logfile), "EVENT:STOP RECORDING").find("Reset to 0 Duration 0:00:10") !=
	      std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Stop stream line states the stream duration", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_stream");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtStream);
	clock.at(seconds(65));
	writer.MarkStop(imtStream);

	CHECK(line_with(read_file(logfile), "EVENT:STOP STREAM @ ")
		      .find("Stream Time Marker Reset to 0 Duration 0:01:05") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Stream duration includes time the recording was paused", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_overlap");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtStream);
	clock.at(seconds(10));
	writer.MarkStart(imtRecording);
	clock.at(seconds(20));
	writer.MarkPauseStart(imtRecordingPauseStart);
	clock.at(seconds(50));
	writer.MarkPauseResume(imtRecordingPauseResume);
	clock.at(seconds(60));
	writer.MarkStop(imtRecording);
	clock.at(seconds(100));
	writer.MarkStop(imtStream);

	auto contents = read_file(logfile);
	CHECK(line_with(contents, "EVENT:STOP RECORDING").find("Reset to 0 Duration 0:00:20") != std::string::npos);
	CHECK(line_with(contents, "EVENT:STOP STREAM").find("Reset to 0 Duration 0:01:40") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Duration uses the configured format including milliseconds", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_millis");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d.%03d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(83) + 456000000ull);
	writer.MarkStop(imtRecording);

	CHECK(line_with(read_file(logfile), "EVENT:STOP RECORDING").find("Reset to 0 Duration 0:01:23.456") !=
	      std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Duration longer than a day keeps counting hours", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_long");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(25 * 3600 + 7));
	writer.MarkStop(imtRecording);

	CHECK(line_with(read_file(logfile), "EVENT:STOP RECORDING").find("Reset to 0 Duration 25:00:07") !=
	      std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Duration leaves out indentation from the format", "[duration][issue80]")
{
	auto format = GENERATE(std::string("\\t%d:%02d:%02d"), std::string("    %d:%02d:%02d"));

	auto dir = make_test_dir("test_infowriter_duration_indent");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, format);

	writer.MarkStart(imtRecording);
	clock.at(seconds(18 * 60 + 30));
	writer.MarkStop(imtRecording);

	auto line = line_with(read_file(logfile), "EVENT:STOP RECORDING");
	CHECK(line.find("Reset to 0 Duration 0:18:30") != std::string::npos);
	CHECK(line.find('\t') == std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Stop line after a split states the segment duration", "[duration][issue80][split]")
{
	auto dir = make_test_dir("test_infowriter_duration_split");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(67));
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	clock.at(seconds(76));
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	CHECK(contents.find("0:01:07 Record Time Marker") != std::string::npos);
	CHECK(line_with(contents, "EVENT:STOP RECORDING").find("Reset to 0 Segment duration 0:00:09") !=
	      std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("A new recording after a split recording states a plain duration again", "[duration][issue80][split]")
{
	auto dir = make_test_dir("test_infowriter_duration_aftersplit");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(67));
	writer.HandleRecordingSplit((dir / "second.mkv").string());
	clock.at(seconds(76));
	writer.MarkStop(imtRecording);

	clock.at(seconds(100));
	writer.MarkStart(imtRecording);
	clock.at(seconds(130));
	writer.MarkStop(imtRecording);

	auto contents = read_file(logfile);
	auto last_stop = contents.substr(contents.rfind("EVENT:STOP RECORDING"));
	CHECK(line_with(last_stop, "EVENT:STOP RECORDING").find("Reset to 0 Duration 0:00:30") != std::string::npos);

	std::filesystem::remove_all(dir);
}

TEST_CASE("Removing the source while recording doesn't state a duration", "[duration][issue80]")
{
	auto dir = make_test_dir("test_infowriter_duration_unknown");
	auto logfile = dir / "log.txt";
	FrozenClock clock;

	InfoWriter writer;
	configure(writer, logfile, "%d:%02d:%02d");

	writer.MarkStart(imtRecording);
	clock.at(seconds(300));
	writer.MarkStop(imtUnknown);

	auto line = line_with(read_file(logfile), "EVENT:STOP RECORDING");
	CHECK(line.size() > 0);
	CHECK(line.find("Duration") == std::string::npos);

	std::filesystem::remove_all(dir);
}
