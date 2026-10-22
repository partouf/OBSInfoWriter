#include <catch2/catch_test_macros.hpp>
#include "FormatUtils.h"

// The synced log filename is derived from the recording path that OBS reports, by swapping the video extension

TEST_CASE("Replace extension of a Linux recording path", "[extension]")
{
	REQUIRE(ReplaceFileExtension("/home/user/2026-03-12 15-00-29.mkv", "txt") ==
		"/home/user/2026-03-12 15-00-29.txt");
}

TEST_CASE("Replace extension of a Windows recording path with forward slashes", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:/Users/Bob/Videos/2026-10-22 20-15-37.mkv", "txt") ==
		"C:/Users/Bob/Videos/2026-10-22 20-15-37.txt");
}

TEST_CASE("Replace extension of a Windows recording path with backslashes", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:\\Users\\Bob\\Videos\\2026-10-22 20-15-37.mp4", "txt") ==
		"C:\\Users\\Bob\\Videos\\2026-10-22 20-15-37.txt");
}

TEST_CASE("Replace extension of a Windows recording path with mixed slashes", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("D:\\Recordings/2026-10-22 20-15-37.mkv", "txt") ==
		"D:\\Recordings/2026-10-22 20-15-37.txt");
}

TEST_CASE("Replace extension of a UNC recording path", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("\\\\nas\\share\\obs\\2026-10-22 20-15-37.mkv", "txt") ==
		"\\\\nas\\share\\obs\\2026-10-22 20-15-37.txt");
}

TEST_CASE("Replace extension when a folder name contains a dot", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:/Users/john.doe/Videos/2026-10-22 20-15-37.mkv", "txt") ==
		"C:/Users/john.doe/Videos/2026-10-22 20-15-37.txt");
}

TEST_CASE("Replace extension when the filename contains extra dots", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:/Videos/stream v2.7.4.mkv", "txt") == "C:/Videos/stream v2.7.4.txt");
}

TEST_CASE("Add extension when the recording filename has none", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:/Videos/recording", "txt") == "C:/Videos/recording.txt");
}

TEST_CASE("Add extension when the filename has none but a folder name contains a dot", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:/Users/john.doe/Videos/recording", "txt") ==
		"C:/Users/john.doe/Videos/recording.txt");
}

TEST_CASE("Add extension when a backslash folder name contains a dot", "[extension][windows]")
{
	REQUIRE(ReplaceFileExtension("C:\\Users\\john.doe\\Videos\\recording", "txt") ==
		"C:\\Users\\john.doe\\Videos\\recording.txt");
}
