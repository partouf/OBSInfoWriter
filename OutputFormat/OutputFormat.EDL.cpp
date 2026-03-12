#include "OutputFormat.EDL.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <algorithm>
#include <cmath>
#include <cstdint>

const int reelnameMaxlength = 5;

OutputFormatEDL::OutputFormatEDL(const InfoWriterSettings &settings, const std::string filename)
	: IOutputFormat(),
	  markercount(0),
	  lastMarker(0),
	  settings(settings),
	  currentFilename(filename)
{
}

void OutputFormatEDL::WriteGFStringToFile(const Groundfloor::String &SData) const
{
	Groundfloor::FileWriter Writer;
	Writer.open(this->currentFilename.c_str(), true);
	Writer.start();
	Writer.add(&SData);

	while (!Writer.isEmpty()) {
		GFMillisleep(10);
	}

	Writer.stopAndWait();
	Writer.close();
}

std::string OutputFormatEDL::MillisToHMSString(const int64_t totalmilliseconds) const
{
	int64_t totalseconds = totalmilliseconds / 1000;
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;
	uint32_t cs = (totalmilliseconds % 1000) / 10;

	const char *timeformat = "%02d:%02d:%02d:%02d";

	char buffer[14];
	sprintf(&buffer[0], timeformat, hr, min, sec, cs);

	return buffer;
}

std::string FilterReelName(const std::string text)
{
	std::string reelname = "";

	for (size_t idx = 0; idx < reelnameMaxlength; idx++) {
		if (idx < text.length()) {
			auto ch = text.at(idx);

			if ((ch >= '0') && (ch <= '9')) {
				// do nothing
			} else if ((ch >= 'A') && (ch <= 'Z')) {
				// do nothing
			} else if ((ch >= 'a') && (ch <= 'z')) {
				// uppercase
				ch += 'A' - 'a';
			} else {
				// invalid character
				ch = '_';
			}

			reelname += ch;
		} else {
			reelname += ' ';
		}
	}

	return reelname;
}

void OutputFormatEDL::writeMarker(const int64_t start_ms, const int64_t stop_ms, const std::string text) const
{
	char crlf[] = GFNATIVENEXTLINE;

	auto markername = FilterReelName(text);

	auto formattedStartTime = MillisToHMSString(start_ms);
	auto formattedStopTime = MillisToHMSString(stop_ms);

	const char *edlformat = "%03d  %s V     C        %s %s %s %s";
	char line[200];
	sprintf(&line[0], edlformat, markercount, markername.c_str(), formattedStartTime.c_str(),
		formattedStopTime.c_str(), formattedStartTime.c_str(), formattedStopTime.c_str());

	Groundfloor::String EdlMarkerLine(line);
	EdlMarkerLine.append_ansi(crlf);

	WriteGFStringToFile(EdlMarkerLine);
}

void OutputFormatEDL::Start()
{
	this->markercount = 1;
	this->lastMarker = 0;
	this->lastMarkerText = "START";
}

void OutputFormatEDL::Stop(const int64_t milliseconds)
{
	// -1 second because if the length is longer than the written video, software might like this EDL even less than it already does
	writeMarker(lastMarker, milliseconds - 1000, lastMarkerText);
}

void OutputFormatEDL::HotkeyMarker(const int64_t milliseconds, const std::string text)
{
	writeMarker(lastMarker, milliseconds, lastMarkerText);

	lastMarker = milliseconds;
	lastMarkerText = text;

	markercount++;
}

void OutputFormatEDL::ScenechangeMarker(const int64_t milliseconds, const std::string scenename)
{
	writeMarker(lastMarker, milliseconds, lastMarkerText);

	lastMarker = milliseconds;
	lastMarkerText = scenename;

	markercount++;
}

void OutputFormatEDL::PausedMarker([[maybe_unused]] const int64_t milliseconds) {}

void OutputFormatEDL::ResumedMarker([[maybe_unused]] const int64_t milliseconds,
				    [[maybe_unused]] const int64_t pauselength_ms)
{
}

void OutputFormatEDL::TextMarker([[maybe_unused]] const std::string text) {}
