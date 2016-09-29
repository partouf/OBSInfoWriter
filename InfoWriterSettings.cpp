#include "InfoWriterSettings.h"

std::string InfoWriterSettings::GetFilename() const
{
   return Filename;
}

std::string InfoWriterSettings::GetFormat() const
{
   return Format;
}

void InfoWriterSettings::SetFilename(std::string filename)
{
   this->Filename = filename;
}

void InfoWriterSettings::SetFormat(std::string format)
{
   this->Format = format;
}
