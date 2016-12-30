#pragma once

#include "InfoWriterSettings.h"
#include <cstdint>

enum InfoMediaType { imtUnknown = 0, imtStream = 1, imtRecording = 2 };
typedef int8_t InfoHotkey;

class InfoWriter
{
private:
   int64_t StartTime;
   InfoWriterSettings Settings;
   bool Started;

   std::string InfoWriter::SecsToHMSString(const int64_t totalseconds) const;
   std::string MilliToHMSString(const int64_t time) const;
   void WriteToFile(const std::string Data) const;
public:
   InfoWriter();

   void MarkStart(const InfoMediaType AType);
   void WriteInfo(const std::string AExtraInfo);
   void WriteInfo(const InfoHotkey AHotkey);
   void MarkStop(const InfoMediaType AType);

   bool HasStarted() const;

   InfoWriterSettings *GetSettings();
};
