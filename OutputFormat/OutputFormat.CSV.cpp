#include "OutputFormat.CSV.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <regex>
#include <cmath>

OutputFormatCSV::OutputFormatCSV(const InfoWriterSettings &settings, const std::string filename)
	: IOutputFormat(),
	  settings(settings),
	  currentFilename(filename)
{
}

std::string OutputFormatCSV::MillisToHMSString(const int64_t totalmilliseconds) const
{
	int64_t totalseconds = totalmilliseconds / 1000;
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;
	uint32_t zzz = totalmilliseconds % 1000;

	std::string format = settings.GetFormat();
	std::string replacement = "\t";
	std::regex tabregex("(\\\\t)");
	format = std::regex_replace(format, tabregex, replacement);
	format += "\0\0\0\0";

	char buffer[1024];
	sprintf(&buffer[0], format.c_str(), hr, min, sec, zzz);

	return buffer;
}

void OutputFormatCSV::WriteGFStringToFile(const std::string filename, const std::string text) const
{
	Groundfloor::String line(text);

	Groundfloor::FileWriter Writer;
	Writer.open(filename.c_str(), true);
	Writer.start();
	Writer.add(&line);

	while (!Writer.isEmpty()) {
		GFMillisleep(10);
	}

	Writer.stopAndWait();
	Writer.close();
}

void OutputFormatCSV::Start() {}

void OutputFormatCSV::Stop([[maybe_unused]] const int64_t milliseconds) {}

void OutputFormatCSV::WriteCSVLine(const int64_t milliseconds, const std::string text) const
{
	char crlf[] = GFNATIVENEXTLINE;

	auto formattedTime = MillisToHMSString(milliseconds);

	std::string line = formattedTime + "," + text + crlf;

	WriteGFStringToFile(currentFilename, line);
}

void OutputFormatCSV::HotkeyMarker(const int64_t milliseconds, const std::string text)
{
	WriteCSVLine(milliseconds, text);
}

void OutputFormatCSV::ScenechangeMarker(const int64_t milliseconds, const std::string scenename)
{
	WriteCSVLine(milliseconds, scenename);
}

void OutputFormatCSV::PausedMarker([[maybe_unused]] const int64_t milliseconds) {}

void OutputFormatCSV::ResumedMarker([[maybe_unused]] const int64_t milliseconds,
				    [[maybe_unused]] const int64_t pauselength_ms)
{
}

void OutputFormatCSV::TextMarker([[maybe_unused]] const std::string text) {}
