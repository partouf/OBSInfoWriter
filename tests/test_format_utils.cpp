#include <catch2/catch_test_macros.hpp>
#include "FormatUtils.h"

TEST_CASE("Format zero milliseconds", "[format]")
{
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d", 0) == "0:00:00");
}

TEST_CASE("Format hours minutes seconds", "[format]")
{
	// 1h 23m 45s = 5025000ms
	int64_t ms = 1 * 3600000 + 23 * 60000 + 45 * 1000;
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d", ms) == "1:23:45");
}

TEST_CASE("Format with milliseconds component", "[format]")
{
	int64_t ms = 1 * 3600000 + 23 * 60000 + 45 * 1000 + 678;
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d.%03d", ms) == "1:23:45.678");
}

TEST_CASE("Format with zero-padded hours", "[format]")
{
	int64_t ms = 5 * 60000 + 30 * 1000;
	REQUIRE(FormatMillisToHMS("%02d:%02d:%02d", ms) == "00:05:30");
}

TEST_CASE("Format with tab replacement", "[format]")
{
	int64_t ms = 60 * 1000;
	REQUIRE(FormatMillisToHMS("%d\\t%02d\\t%02d", ms) == "0\t01\t00");
}

TEST_CASE("Format milliseconds only partially used", "[format]")
{
	int64_t ms = 500;
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d.%03d", ms) == "0:00:00.500");
}

TEST_CASE("Format large duration", "[format]")
{
	// 10h 0m 0s
	int64_t ms = 10 * 3600000;
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d", ms) == "10:00:00");
}

TEST_CASE("Format with %s returns raw format", "[format]")
{
	REQUIRE(FormatMillisToHMS("%s", 0) == "%s");
}

TEST_CASE("Format with %n returns raw format", "[format]")
{
	REQUIRE(FormatMillisToHMS("%d:%02d:%02d %n", 5000) == "%d:%02d:%02d %n");
}

TEST_CASE("Format with %p returns raw format", "[format]")
{
	REQUIRE(FormatMillisToHMS("%p", 0) == "%p");
}

TEST_CASE("Empty format does not crash", "[format]")
{
	REQUIRE_NOTHROW(FormatMillisToHMS("", 5000));
}

TEST_CASE("Format with no specifiers returns literal", "[format]")
{
	REQUIRE(FormatMillisToHMS("hello", 5000) == "hello");
}

TEST_CASE("Format with 5 specifiers returns raw format", "[format]")
{
	std::string fmt = "%d:%02d:%02d.%03d.%d";
	REQUIRE(FormatMillisToHMS(fmt, 5000) == fmt);
}

TEST_CASE("Count specifiers with 3 specifiers", "[format]")
{
	REQUIRE(CountFormatSpecifiers("%d:%02d:%02d") == 3);
}

TEST_CASE("Count specifiers with 4 specifiers", "[format]")
{
	REQUIRE(CountFormatSpecifiers("%d:%02d:%02d.%03d") == 4);
}

TEST_CASE("Count specifiers with 5 specifiers", "[format]")
{
	REQUIRE(CountFormatSpecifiers("%d:%02d:%02d.%03d.%d") == 5);
}

TEST_CASE("Count specifiers ignores literal percent", "[format]")
{
	REQUIRE(CountFormatSpecifiers("%%d:%02d:%02d") == 2);
}
