#pragma once

#include "InfoWriterSettings.h"

class InfoWriter
{
private:
   __int64 StartTime;
   InfoWriterSettings Settings;
   bool Started;

   std::string InfoWriter::SecsToHMSString(__int64 totalseconds);
   std::string MilliToHMSString(__int64 time);
   void WriteToFile(std::string Data);
public:
   InfoWriter();

   void MarkStart();
   void WriteInfo();
   void MarkStop();

   bool HasStarted();

   InfoWriterSettings *GetSettings();
};
