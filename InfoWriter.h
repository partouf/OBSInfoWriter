#pragma once

#include "InfoWriterSettings.h"
#include <cstdint>
#include <Groundfloor/Molecules/String.h>

enum InfoMediaType { imtUnknown = 0, imtStream = 1, imtRecording = 2, 
					 imtRecordingPauseStart = 3, imtRecordingPauseResume = 4
};
typedef int8_t InfoHotkey;

class InfoWriter
{
private:
   int64_t StartTime;
   int64_t StartRecordTime;
   int64_t StartStreamTime;
   int64_t PausedTotalTime;
   int64_t PausedStartTime;

   InfoWriterSettings Settings;
   bool StreamStarted;
   bool RecordStarted;  
   bool ShowStreaming;
   bool Paused;

   std::string CurrentFilename;

   void InitCurrentFilename(int64_t timestamp);
   std::string SecsToHMSString(const int64_t totalseconds) const;
   std::string MilliToHMSString(const int64_t time) const;
   void WriteToFile(const std::string Data) const;
   void WriteDblLineToFile(const std::string Data) const;
   void WriteGFStringToFile(const Groundfloor::String &SData) const;
public:
   InfoWriter();

   void MarkStart(const InfoMediaType AType);
   void MarkPauseStart(InfoMediaType AType);
   void MarkPauseResume(InfoMediaType AType);
   void WriteInfo(const std::string AExtraInfo) const;
   void WriteInfo(const InfoHotkey AHotkey) const;
   void MarkStop(const InfoMediaType AType);

   bool HasStarted() const;
   bool IsStreaming() const;
   bool ShowStreamOutput() const;
   void SetShowStreamOutput(bool logchanges);

   std::string NowTimeStamp() const;

   InfoWriterSettings *GetSettings();
};
