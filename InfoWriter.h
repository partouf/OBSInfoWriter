#pragma once

#ifndef FAKEOBS
#include <obs.hpp>
#else
typedef void* calldata_t;

class OBSSignal
{
public:
   void Connect(void *handler_, const char *signal_, void *callback_, void *param_)
   {
   }
};
#endif

#include "InfoWriterSettings.h"

class InfoWriter
{
private:
   __int64 StartTime;
   InfoWriterSettings Settings;
   bool Started;

   OBSSignal OutputStarting;
   OBSSignal OutputStopping;

   static void OnOutputStarting(void *data, calldata_t *calldata);
   static void OnOutputStopping(void *param, calldata_t *calldata);

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
