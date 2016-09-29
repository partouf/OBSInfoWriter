#include "InfoWriter.h"

#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <cstdint>

const char *c_TimestampNotation = "%Y-%m-%d %H:%M:%S";

InfoWriter::InfoWriter()
{
   StartTime = 0;
   Started = false;
}

std::string InfoWriter::SecsToHMSString(const int64_t totalseconds) const
{
   uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
   uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
   uint32_t sec = totalseconds % 60;

   auto Format = Settings.GetFormat() + "\0\0\0\0";

   char buffer[1024];
   sprintf(&buffer[0], Format.c_str(), hr, min, sec);

   return buffer;
}

std::string InfoWriter::MilliToHMSString(const int64_t time) const
{
   uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

   return SecsToHMSString(totalseconds);
}

void InfoWriter::WriteToFile(const std::string Data) const
{
   char crlf[] = GFNATIVENEXTLINE;

   Groundfloor::String SData(Data.c_str());
   SData.append_ansi(crlf);

   Groundfloor::FileWriter Writer;
   Writer.open(Settings.GetFilename().c_str(), true);
   Writer.start();
   Writer.add(&SData);

   while (!Writer.isEmpty())
   {
      GFMillisleep(10);
   }

   Writer.stopAndWait();
   Writer.close();
}

void InfoWriter::WriteInfo(const std::string AExtraInfo)
{
   if (Started)
   {
      auto CurrentTime = Groundfloor::GetTimestamp();
      auto Info = SecsToHMSString(CurrentTime - StartTime);

      if (AExtraInfo.length() != 0)
      {
         Info += " - ";
         Info += AExtraInfo;
      }

      WriteToFile(Info);
   }
}

void InfoWriter::WriteInfo()
{
   WriteInfo("");
}

void InfoWriter::MarkStart(InfoMediaType AType)
{
   StartTime = Groundfloor::GetTimestamp();
   Started = true;

   auto MarkStr = Groundfloor::TimestampToStr(c_TimestampNotation, StartTime);

   switch (AType) {
   case imtUnknown:
      MarkStr->prepend_ansi("START @ ");
      break;
   case imtStream:
      MarkStr->prepend_ansi("START STREAM @ ");
      break;
   case imtRecording:
      MarkStr->prepend_ansi("START RECORDING @ ");
      break;
   }

   WriteToFile(MarkStr->getValue());

   delete MarkStr;
}

void InfoWriter::MarkStop(InfoMediaType AType)
{
   StartTime = 0;
   Started = false;

   auto Now = Groundfloor::GetTimestamp();
   auto MarkStr = Groundfloor::TimestampToStr(c_TimestampNotation, Now);

   switch (AType) {
   case imtUnknown:
      MarkStr->prepend_ansi("STOP @ ");
      break;
   case imtStream:
      MarkStr->prepend_ansi("STOP STREAM @ ");
      break;
   case imtRecording:
      MarkStr->prepend_ansi("STOP RECORDING @ ");
      break;
   }

   WriteToFile(MarkStr->getValue());

   delete MarkStr;
}

bool InfoWriter::HasStarted() const
{
   return Started;
}

InfoWriterSettings * InfoWriter::GetSettings()
{
   return &Settings;
}
