#include <catch2/catch_test_macros.hpp>
#include "InfoWriter.h"
#include <filesystem>
#include <stdexcept>

static InfoWriter make_writer(const std::string &filename)
{
	InfoWriter writer;
	auto *settings = writer.GetSettings();
	settings->SetFilename(filename);
	settings->SetFormat("%d:%02d:%02d");
	return writer;
}

// Issue #82: invalid strftime specifiers crash OBS because
// Groundfloor::TimestampToStr throws std::runtime_error and
// InfoWriter::InitCurrentFilename does not catch it.

TEST_CASE("Invalid strftime specifier %s in filename throws", "[filename][issue82]")
{
	auto writer = make_writer("./log %s.txt");
	REQUIRE_THROWS_AS(writer.MarkStart(imtStream), std::runtime_error);
}

TEST_CASE("Invalid strftime specifier %k in filename throws", "[filename][issue82]")
{
	auto writer = make_writer("./log %k.txt");
	REQUIRE_THROWS_AS(writer.MarkStart(imtStream), std::runtime_error);
}

TEST_CASE("Incomplete % at end of filename throws", "[filename][issue82]")
{
	auto writer = make_writer("log %");
	REQUIRE_THROWS_AS(writer.MarkStart(imtStream), std::runtime_error);
}

TEST_CASE("Valid strftime specifiers in filename do not throw", "[filename]")
{
	auto writer = make_writer("/tmp/test_infowriter_valid_%Y-%m-%d_%H%M%S.txt");
	REQUIRE_NOTHROW(writer.MarkStart(imtStream));
	writer.MarkStop(imtStream);
}

TEST_CASE("Filename without specifiers does not throw", "[filename]")
{
	std::string tmpfile = "/tmp/test_infowriter_plain.txt";
	auto writer = make_writer(tmpfile);
	REQUIRE_NOTHROW(writer.MarkStart(imtStream));
	writer.MarkStop(imtStream);
	std::filesystem::remove(tmpfile);
}
