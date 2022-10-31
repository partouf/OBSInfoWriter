#include "OutputFormat.SRT.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <regex>
#include <cmath>

const char *c_TimestampNotation = "%Y-%m-%d %H:%M:%S";

OutputFormatSRT::OutputFormatSRT(const InfoWriterSettings &settings,
				 const std::string filename)
	: IOutputFormat(),
	  settings(settings),
	  currentFilename(filename),
	  subtitleCounter(0),
	  canAppend(false)
{
}

std::string OutputFormatSRT::SecsToHMSString(const int64_t totalseconds) const
{
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;

	std::string format = "%02d:%02d:%02d,000";
	std::string replacement = "\t";
	format += "\0\0\0\0";

	char buffer[1024];
	sprintf(&buffer[0], format.c_str(), hr, min, sec);

	return buffer;
}

void OutputFormatSRT::WriteGFStringToFile(const std::string filename,
					  const std::string text) const
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

void OutputFormatSRT::WriteLines(const int64_t timestamp,
				 const std::string text)
{
	char crlf[] = GFNATIVENEXTLINE;

	char buffer[1024];
	sprintf(&buffer[0], "%d", this->subtitleCounter);

	std::string line1 = buffer;
	std::string line2 = SecsToHMSString(timestamp) + " --> " +
			    SecsToHMSString(timestamp + 5);

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
		Groundfloor::TimestampToStr(c_TimestampNotation,
					    Groundfloor::GetTimestamp())
			->getValue();
	std::string line = "START @ " + timestr;
	WriteLines(0, line);

	this->subtitleCounter++;
}

void OutputFormatSRT::Stop(const int64_t timestamp)
{
	std::string timestr =
		Groundfloor::TimestampToStr(c_TimestampNotation,
					    Groundfloor::GetTimestamp())
			->getValue();
	std::string line = "STOP @ " + timestr;
	WriteLines(timestamp, line);

	this->subtitleCounter++;
}

void OutputFormatSRT::HotkeyMarker(const int64_t timestamp,
				   const std::string text)
{
	WriteLines(timestamp, text);

	this->subtitleCounter++;
}

void OutputFormatSRT::ScenechangeMarker(const int64_t timestamp,
					const std::string scenename)
{
	WriteLines(timestamp, scenename);

	this->subtitleCounter++;
}

void OutputFormatSRT::PausedMarker(const int64_t timestamp) {}

void OutputFormatSRT::ResumedMarker(const int64_t timestamp,
				    const int64_t pauselength)
{
}

void OutputFormatSRT::TextMarker(const std::string text) {}
