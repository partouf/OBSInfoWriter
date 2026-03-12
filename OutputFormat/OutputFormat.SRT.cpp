#include "OutputFormat.SRT.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <regex>
#include <cmath>

const char *c_TimestampNotation_SRT = "%Y-%m-%d %H:%M:%S";

OutputFormatSRT::OutputFormatSRT(const InfoWriterSettings &settings, const std::string filename)
	: IOutputFormat(),
	  settings(settings),
	  currentFilename(filename),
	  subtitleCounter(0),
	  canAppend(false)
{
}

std::string OutputFormatSRT::MillisToHMSString(const int64_t totalmilliseconds) const
{
	int64_t totalseconds = totalmilliseconds / 1000;
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;
	uint32_t zzz = totalmilliseconds % 1000;

	char buffer[1024];
	sprintf(&buffer[0], "%02d:%02d:%02d,%03d", hr, min, sec, zzz);

	return buffer;
}

void OutputFormatSRT::WriteGFStringToFile(const std::string filename, const std::string text) const
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

void OutputFormatSRT::WriteLines(const int64_t milliseconds, const std::string text)
{
	char crlf[] = GFNATIVENEXTLINE;

	char buffer[1024];
	sprintf(&buffer[0], "%d", this->subtitleCounter);

	std::string line1 = buffer;
	std::string line2 = MillisToHMSString(milliseconds) + " --> " + MillisToHMSString(milliseconds + 5000);

	WriteGFStringToFile(currentFilename, line1 + crlf);
	WriteGFStringToFile(currentFilename, line2 + crlf);

	if (text.length() > 0) {
		WriteGFStringToFile(currentFilename, text + crlf);
	}

	WriteGFStringToFile(currentFilename, crlf);
}

void OutputFormatSRT::Start()
{
	this->subtitleCounter = 1;
	std::string timestr =
		Groundfloor::TimestampToStr(c_TimestampNotation_SRT, Groundfloor::GetTimestamp())->getValue();
	std::string line = "START @ " + timestr;
	WriteLines(0, line);

	this->subtitleCounter++;
}

void OutputFormatSRT::Stop(const int64_t milliseconds)
{
	std::string timestr =
		Groundfloor::TimestampToStr(c_TimestampNotation_SRT, Groundfloor::GetTimestamp())->getValue();
	std::string line = "STOP @ " + timestr;
	WriteLines(milliseconds, line);

	this->subtitleCounter++;
}

void OutputFormatSRT::HotkeyMarker(const int64_t milliseconds, const std::string text)
{
	WriteLines(milliseconds, text);

	this->subtitleCounter++;
}

void OutputFormatSRT::ScenechangeMarker(const int64_t milliseconds, const std::string scenename)
{
	WriteLines(milliseconds, scenename);

	this->subtitleCounter++;
}

void OutputFormatSRT::PausedMarker([[maybe_unused]] const int64_t milliseconds) {}

void OutputFormatSRT::ResumedMarker([[maybe_unused]] const int64_t milliseconds,
				    [[maybe_unused]] const int64_t pauselength_ms)
{
}

void OutputFormatSRT::TextMarker([[maybe_unused]] const std::string text) {}
