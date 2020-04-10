#pragma once

#include "../OutputFormat.h"
#include "../InfoWriter.h"
#include <string>

class OutputFormatCSV final : public IOutputFormat
{
private:
   const InfoWriterSettings& settings;
   std::string currentFilename;
   std::string OutputFormatCSV::SecsToHMSString(const int64_t totalseconds) const;
   void WriteCSVLine(const int64_t timestamp, const std::string text) const;

public:
   OutputFormatCSV(const InfoWriterSettings& settings, const std::string filename);

   void Start() override;
   void Stop(const int64_t timestamp) override;
   void HotkeyMarker(const int64_t timestamp, const std::string text) override;
   void ScenechangeMarker(const int64_t timestamp, const std::string scenename) override;
   void PausedMarker(const int64_t timestamp) override;
   void ResumedMarker(const int64_t timestamp, const int64_t pauselength) override;
   void TextMarker(const std::string text) override;
};
