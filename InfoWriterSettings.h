#pragma once

#include <string>

class InfoWriterSettings
{
protected:
   std::string Filename;
   std::string Format;
public:
   std::string GetFilename();
   std::string GetFormat();

   void SetFilename(std::string filename);
   void SetFormat(std::string format);
};
