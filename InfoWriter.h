#pragma once

#include "InfoWriterSettings.h"

enum InfoMediaType { imtUnknown = 0, imtStream = 1, imtRecording = 2 };

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

   void MarkStart(InfoMediaType AType);
   void WriteInfo();
   void MarkStop(InfoMediaType AType);

   bool HasStarted();

   InfoWriterSettings *GetSettings();
};
