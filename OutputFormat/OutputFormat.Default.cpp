#include "OutputFormat.Default.h"
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <regex>
#include <cmath>
#include <cstdint>

const char *c_OutputDefaultTimestampNotation = "%Y-%m-%d %H:%M:%S";

OutputFormatDefault::OutputFormatDefault(const InfoWriterSettings &settings,
					 const std::string filename)
	: IOutputFormat(),
	  settings(settings),
	  currentFilename(filename),
	  startTime(0)
{
}

std::string
OutputFormatDefault::SecsToHMSString(const int64_t totalseconds) const
{
	uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
	uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
	uint32_t sec = totalseconds % 60;

	std::string format = settings.GetFormat();
	std::string replacement = "\t";
	std::regex tabregex("(\\\\t)");
	format = std::regex_replace(format, tabregex, replacement);
	format += "\0\0\0\0";

	char buffer[1024];
	sprintf(&buffer[0], format.c_str(), hr, min, sec);

	return buffer;
}

std::string OutputFormatDefault::MilliToHMSString(const int64_t time) const
{
	uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

	return SecsToHMSString(totalseconds);
}

void OutputFormatDefault::WriteToFile(const std::string Data) const
{
	char crlf[] = GFNATIVENEXTLINE;

	Groundfloor::String SData(Data.c_str());
	SData.append_ansi(crlf);

	WriteGFStringToFile(SData);
}

void OutputFormatDefault::WriteDblLineToFile(const std::string Data) const
{
	char crlf[] = GFNATIVENEXTLINE;

	Groundfloor::String SData(Data.c_str());
	SData.append_ansi(crlf);
	SData.append_ansi(crlf);

	WriteGFStringToFile(SData);
}

void OutputFormatDefault::WriteGFStringToFile(
	const Groundfloor::String &SData) const
{
	Groundfloor::FileWriter Writer;
	Writer.open(currentFilename.c_str(), true);
	Writer.start();
	Writer.add(&SData);

	while (!Writer.isEmpty()) {
		GFMillisleep(10);
	}

	Writer.stopAndWait();
	Writer.close();
}

void OutputFormatDefault::Start()
{
	startTime = Groundfloor::GetTimestamp();
}

void OutputFormatDefault::Stop(const int64_t timestamp) {}

void OutputFormatDefault::HotkeyMarker(const int64_t timestamp,
				       const std::string text)
{
	if (this->settings.GetShouldLogHotkeySpecifics()) {
		if (this->settings.GetShouldLogAbsoluteTime()) {
			auto MarkStr = Groundfloor::TimestampToStr(
				c_OutputDefaultTimestampNotation,
				startTime + timestamp);
			auto hotkey_text =
				"HOTKEY:" + text + " @ " + MarkStr->getValue();
			WriteToFile(hotkey_text);
			delete MarkStr;
		} else {
			auto hotkey_text = "HOTKEY:" + text;
			WriteToFile(hotkey_text);
		}
	}
}

void OutputFormatDefault::ScenechangeMarker(const int64_t timestamp,
					    const std::string scenename)
{
	auto MarkStr = Groundfloor::TimestampToStr(
		c_OutputDefaultTimestampNotation, startTime + timestamp);

	if (scenename == "") {
		WriteToFile("EVENT:SCENE CHANGED @ " +
			    std::string(MarkStr->getValue()));
	} else {
		WriteToFile("EVENT:SCENE CHANGED TO " + scenename + " @ " +
			    std::string(MarkStr->getValue()));
	}

	delete MarkStr;
}

void OutputFormatDefault::PausedMarker(const int64_t timestamp)
{
	auto MarkStr = Groundfloor::TimestampToStr(
		c_OutputDefaultTimestampNotation, startTime + timestamp);

	WriteToFile("EVENT:RECORDING PAUSED @ " +
		    std::string(MarkStr->getValue()));

	delete MarkStr;
}

void OutputFormatDefault::ResumedMarker(const int64_t timestamp,
					const int64_t pauselength)
{
	auto MarkStr = Groundfloor::TimestampToStr(
		c_OutputDefaultTimestampNotation, startTime + timestamp);

	std::string Info;
	Info = "EVENT:RECORDING RESUMED @ " + std::string(MarkStr->getValue()) +
	       " (paused lasted for ";
	Info += SecsToHMSString(pauselength);
	Info += " seconds)";

	delete MarkStr;

	WriteToFile(Info);
}

void OutputFormatDefault::TextMarker(const std::string text)
{
	WriteToFile(text);
}
