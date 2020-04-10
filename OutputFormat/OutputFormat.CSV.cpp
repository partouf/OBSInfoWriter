#include "OutputFormat.CSV.h"
#include <Groundfloor/Materials/FileWriter.h>
#include <regex>
#include <cmath>

OutputFormatCSV::OutputFormatCSV(const InfoWriterSettings& settings, const std::string filename) : IOutputFormat(), settings(settings), currentFilename(filename)
{
}

std::string OutputFormatCSV::SecsToHMSString(const int64_t totalseconds) const
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

void WriteGFStringToFile(const std::string filename, const std::string text)
{
   Groundfloor::String line(text);

   Groundfloor::FileWriter Writer;
   Writer.open(filename.c_str(), true);
   Writer.start();
   Writer.add(&line);

   while (!Writer.isEmpty())
   {
      GFMillisleep(10);
   }

   Writer.stopAndWait();
   Writer.close();
}

void OutputFormatCSV::Start()
{
}

void OutputFormatCSV::Stop(const int64_t timestamp)
{
}

void OutputFormatCSV::WriteCSVLine(const int64_t timestamp, const std::string text) const
{
   char crlf[] = GFNATIVENEXTLINE;

   auto formattedTime = SecsToHMSString(timestamp);

   std::string line = formattedTime + "," + text + crlf;

   WriteGFStringToFile(currentFilename, line);
}

void OutputFormatCSV::HotkeyMarker(const int64_t timestamp, const std::string text)
{
   WriteCSVLine(timestamp, text);
}

void OutputFormatCSV::ScenechangeMarker(const int64_t timestamp, const std::string scenename)
{
   WriteCSVLine(timestamp, scenename);
}

void OutputFormatCSV::PausedMarker(const int64_t timestamp)
{
}

void OutputFormatCSV::ResumedMarker(const int64_t timestamp, const int64_t pauselength)
{
}

void OutputFormatCSV::TextMarker(const std::string text)
{
}
