#include "InfoWriter.h"

#include "InfoWriterSettings.h"
#include <Groundfloor/Atoms/Defines.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>

InfoWriter::InfoWriter()
{
   StartTime = 0;
   Started = false;
}

std::string InfoWriter::SecsToHMSString(__int64 totalseconds)
{
   uint32_t hr = (uint32_t)trunc(totalseconds / 60.0 / 60.0);
   uint32_t min = (uint32_t)trunc(totalseconds / 60.0) - (hr * 60);
   uint32_t sec = totalseconds % 60;

   auto Format = Settings.GetFormat() + "\0\0\0\0";

   char buffer[1024];
   sprintf(&buffer[0], Format.c_str(), hr, min, sec);

   return buffer;
}

std::string InfoWriter::MilliToHMSString(__int64 time)
{
   uint32_t totalseconds = (uint32_t)trunc(time / 1000.0);

   return SecsToHMSString(totalseconds);
}

void InfoWriter::WriteToFile(std::string Data)
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

void InfoWriter::WriteInfo()
{
   if (Started)
   {
      auto CurrentTime = Groundfloor::GetTimestamp();
      auto Info = SecsToHMSString(CurrentTime - StartTime);

      WriteToFile(Info);
   }
}

void InfoWriter::MarkStart()
{
   StartTime = Groundfloor::GetTimestamp();
   Started = true;

   auto MarkStr = Groundfloor::TimestampToStr("%Y-%m-%d %H:%m:%S", StartTime);
   MarkStr->prepend_ansi("START @ ");

   WriteToFile(MarkStr->getValue());

   delete MarkStr;
}

void InfoWriter::MarkStop()
{
   StartTime = 0;
   Started = false;

   auto Now = Groundfloor::GetTimestamp();
   auto MarkStr = Groundfloor::TimestampToStr("%Y-%m-%d %H:%m:%S", Now);
   MarkStr->prepend_ansi("STOP @ ");

   WriteToFile(MarkStr->getValue());

   delete MarkStr;
}

bool InfoWriter::HasStarted()
{
   return Started;
}

InfoWriterSettings * InfoWriter::GetSettings()
{
   return &Settings;
}
