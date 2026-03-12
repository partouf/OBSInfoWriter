#include "FormatUtils.h"
#include <cmath>
#include <cstdio>
#include <regex>

bool HasUnsafeFormatSpecifiers(const std::string &format)
{
	// Reject format specifiers that expect non-integer arguments
	// since we only pass uint32_t values to sprintf.
	// Matches: %s, %n, %p, %f, %e, %g, %a (and uppercase variants)
	// while allowing flags, width, and precision between % and the specifier.
	std::regex unsafe_regex("%[^%]*[snpfFeEgGaA]");
	return std::regex_search(format, unsafe_regex);
}

int CountFormatSpecifiers(const std::string &format)
{
	// Count printf format specifiers, excluding %% (literal percent)
	// First remove all %% so they don't interfere with matching
	std::string cleaned = std::regex_replace(format, std::regex("%%"), "");
	std::regex specifier_regex("%[^%]*[diouxXcnspeEfFgGaA]");
	auto begin = std::sregex_iterator(cleaned.begin(), cleaned.end(), specifier_regex);
	auto end = std::sregex_iterator();
	return (int)std::distance(begin, end);
}

static const int MaxFormatSpecifiers = 4; // hr, min, sec, zzz

std::string FormatMillisToHMS(const std::string &format, int64_t totalmilliseconds)
{
	if (HasUnsafeFormatSpecifiers(format))
		return format;

	if (CountFormatSpecifiers(format) > MaxFormatSpecifiers)
		return format;

	int64_t totalseconds = totalmilliseconds / 1000;
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;
	uint32_t zzz = totalmilliseconds % 1000;

	std::string fmt = format;
	std::string replacement = "\t";
	std::regex tabregex("(\\\\t)");
	fmt = std::regex_replace(fmt, tabregex, replacement);
	fmt += "\0\0\0\0";

	char buffer[1024];
	sprintf(&buffer[0], fmt.c_str(), hr, min, sec, zzz);

	return buffer;
}
