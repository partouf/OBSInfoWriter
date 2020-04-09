#include "InfoWriter.h"

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <cstdint>
#include <cmath>
#include <regex>

const char *c_TimestampNotation = "%Y-%m-%d %H:%M:%S";

InfoWriter::InfoWriter()
{
   StartTime = 0;
   StartRecordTime = 0;
   StartStreamTime = 0;
   PausedTotalTime = 0;
   PausedStartTime = 0;
   StreamStarted = false;
   RecordStarted = false;
   ShowStreaming = false;
   Paused = false;
}

std::string InfoWriter::SecsToHMSString(const int64_t totalseconds) const
{
   uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
   uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
   uint32_t sec = totalseconds % 60;

   std::string format = Settings.GetFormat();
   std::string replacement = "\t";
   std::regex tabregex("(\\\\t)");
   format = std::regex_replace(format, tabregex, replacement);
   format += "\0\0\0\0";

   char buffer[1024];
   sprintf(&buffer[0], format.c_str(), hr, min, sec);

   return buffer;
}

std::string InfoWriter::MilliToHMSString(const int64_t time) const
{
   uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

   return SecsToHMSString(totalseconds);
}

std::string InfoWriter::NowTimeStamp() const
{
	auto NowStr = Groundfloor::TimestampToStr(c_TimestampNotation, StartTime);
	return NowStr->getValue();
}

void InfoWriter::WriteToFile(const std::string Data) const
{
   char crlf[] = GFNATIVENEXTLINE;

   Groundfloor::String SData(Data.c_str());
   SData.append_ansi(crlf);

   WriteGFStringToFile(SData);
}

void InfoWriter::WriteDblLineToFile(const std::string Data) const
{
   char crlf[] = GFNATIVENEXTLINE;

   Groundfloor::String SData(Data.c_str());
   SData.append_ansi(crlf);
   SData.append_ansi(crlf);

   WriteGFStringToFile(SData);
}

void InfoWriter::WriteGFStringToFile(const Groundfloor::String &SData) const
{
   Groundfloor::FileWriter Writer;
   Writer.open(CurrentFilename.c_str(), true);
   Writer.start();
   Writer.add(&SData);

   while (!Writer.isEmpty())
   {
      GFMillisleep(10);
   }

   Writer.stopAndWait();
   Writer.close();
}

void InfoWriter::WriteInfo(const std::string AExtraInfo) const
{
	std::string Info;
	auto CurrentTime = Groundfloor::GetTimestamp();
	auto tmpTime = SecsToHMSString(0);

	auto PausedTmpTime = PausedTotalTime;
	if (Paused) PausedTmpTime = PausedTotalTime + (CurrentTime - PausedStartTime);

	if (RecordStarted)
	{
		tmpTime = SecsToHMSString((CurrentTime - StartRecordTime) - PausedTmpTime);
	}

	std::string record_info = tmpTime;
	tmpTime = SecsToHMSString(0);
	if (StreamStarted)
	{
		tmpTime = SecsToHMSString(CurrentTime - StartStreamTime);
	}
	auto stream_info = tmpTime;

	record_info += " Record Time Marker";
	stream_info += " Stream Time Marker";

	if (!RecordStarted) record_info += " (not recording)";
	if (Paused) record_info += " (recording is paused)";
	if (!StreamStarted) stream_info += " (not streaming)";

	record_info += "\n";
	stream_info += "\n";

	if (AExtraInfo.length() != 0)
	{
		Info = "";
		Info += AExtraInfo;
		Info += "\n";
	}

	Info += record_info;
	if (ShowStreaming) Info += stream_info;

	WriteToFile(Info);
}

void InfoWriter::WriteInfo(const InfoHotkey AHotkey) const
{
	auto Now = Groundfloor::GetTimestamp();
	auto MarkStr = Groundfloor::TimestampToStr(c_TimestampNotation, Now);
	auto hotkey_text = Settings.GetHotkeyText(AHotkey);  

    hotkey_text = "HOTKEY:" + hotkey_text + " @ " + MarkStr->getValue();

    WriteInfo(hotkey_text);
    delete MarkStr;
}

void InfoWriter::InitCurrentFilename(int64_t timestamp)
{
  CurrentFilename = Settings.GetFilename();
  if (CurrentFilename.find('%') != 0) {
      auto filename = Groundfloor::TimestampToStr(CurrentFilename.c_str(), StartTime);
      CurrentFilename = filename->getValue();
      delete filename;
  }
}

void InfoWriter::MarkStart(InfoMediaType AType)
{
   StartTime = Groundfloor::GetTimestamp();
   InitCurrentFilename(StartTime);

   auto MarkStr = Groundfloor::TimestampToStr(c_TimestampNotation, StartTime);

   switch (AType) {
   case imtStream:
	   StartStreamTime = Groundfloor::GetTimestamp();
	   StreamStarted = true;
	   if (ShowStreaming)
	   {
		   MarkStr->prepend_ansi("EVENT:START STREAM @ ");
	   }
	  WriteInfo(MarkStr->getValue());
      break;
   case imtUnknown:
      MarkStr->prepend_ansi(" (WARNING:Unsure how we STARTED) ");
   case imtRecording:
      MarkStr->prepend_ansi("EVENT:START RECORDING @ ");
	  StartRecordTime = Groundfloor::GetTimestamp();
	  RecordStarted = true;
	  Paused = false;
	  WriteInfo(MarkStr->getValue());
      break;
   }

   delete MarkStr;
}

void InfoWriter::MarkStop(InfoMediaType AType)
{
	auto Now = Groundfloor::GetTimestamp();
	auto MarkStr = Groundfloor::TimestampToStr(c_TimestampNotation, Now);

	switch (AType) {
	case imtStream:
		MarkStr->prepend_ansi("EVENT:STOP STREAM @ ");
		MarkStr->append(" Stream Time Marker Reset to 0");	
		StreamStarted = false;
		StartStreamTime = 0;
		break;
	case imtUnknown:
	case imtRecording:
		MarkStr->prepend_ansi("EVENT:STOP RECORDING @ ");
		MarkStr->append(" Record Time Marker Reset to 0");
		StartRecordTime = 0;  //reset times 
		PausedTotalTime = 0;
		Paused = false;
		RecordStarted = false;
		break;
	}

	WriteInfo(MarkStr->getValue());

	delete MarkStr;
}

//TheTawnyFool addition for pausing
void InfoWriter::MarkPauseStart(InfoMediaType AType)
{
   Paused = true;
   PausedStartTime = Groundfloor::GetTimestamp();

   WriteInfo("EVENT:RECORDING PAUSED @ " + NowTimeStamp() );
}    

void InfoWriter::MarkPauseResume(InfoMediaType AType)
{
	Paused = false;
	auto CurrentTime = Groundfloor::GetTimestamp();
	PausedTotalTime += (CurrentTime - PausedStartTime);

	std::string Info;
	Info = "EVENT: RECORDING RESUMED @ " + NowTimeStamp() + "(paused lasted for ";
	Info += SecsToHMSString(CurrentTime - PausedStartTime);
	Info += " seconds)";

	WriteInfo(Info);
}

bool InfoWriter::HasStarted() const
{
	if (StreamStarted) return true;
	if (RecordStarted) return true;
	return false;
}

bool InfoWriter::IsStreaming() const
{
	return StreamStarted;
}

bool InfoWriter::ShowStreamOutput() const
{
	return ShowStreaming;
}

void InfoWriter::SetShowStreamOutput(bool logchanges)
{
	ShowStreaming = logchanges;
}

InfoWriterSettings *InfoWriter::GetSettings()
{
   return &Settings;
}
