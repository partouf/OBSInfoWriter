#include "OutputFormat.EDL.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <algorithm>
#include <cmath>
#include <cstdint>

const int reelnameMaxlength = 5;

OutputFormatEDL::OutputFormatEDL(const InfoWriterSettings &settings,
				 const std::string filename)
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

std::string OutputFormatEDL::SecsToHMSString(const int64_t totalseconds) const
{
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;

	const char *timeformat = "%02d:%02d:%02d:%02d";

	char buffer[14];
	sprintf(&buffer[0], timeformat, hr, min, sec, 0);

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

void OutputFormatEDL::writeMarker(const int64_t start, const int64_t stop,
				  const std::string text) const
{
	char crlf[] = GFNATIVENEXTLINE;

	auto markername = FilterReelName(text);

	auto formattedStartTime = SecsToHMSString(start);
	auto formattedStopTime = SecsToHMSString(stop);

	const char *edlformat = "%03d  %s V     C        %s %s %s %s";
	char line[200];
	sprintf(&line[0], edlformat, markercount, markername.c_str(),
		formattedStartTime.c_str(), formattedStopTime.c_str(),
		formattedStartTime.c_str(), formattedStopTime.c_str());

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

void OutputFormatEDL::Stop(const int64_t timestamp)
{
	// -1 second because if the length is longer than the written video, software might like this EDL even less than it already does
	writeMarker(lastMarker, timestamp - 1, lastMarkerText);
}

void OutputFormatEDL::HotkeyMarker(const int64_t timestamp,
				   const std::string text)
{
	writeMarker(lastMarker, timestamp, lastMarkerText);

	lastMarker = timestamp;
	lastMarkerText = text;

	markercount++;
}

void OutputFormatEDL::ScenechangeMarker(const int64_t timestamp,
					const std::string scenename)
{
	writeMarker(lastMarker, timestamp, lastMarkerText);

	lastMarker = timestamp;
	lastMarkerText = scenename;

	markercount++;
}

void OutputFormatEDL::PausedMarker(const int64_t timestamp) {}

void OutputFormatEDL::ResumedMarker(const int64_t timestamp,
				    const int64_t pauselength)
{
}

void OutputFormatEDL::TextMarker(const std::string text) {}
