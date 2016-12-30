#include "InfoWriterSettings.h"

std::string InfoWriterSettings::GetFilename() const
{
   return Filename;
}

std::string InfoWriterSettings::GetFormat() const
{
   return Format;
}

std::string InfoWriterSettings::GetHotkeyText(const int hotkeynum) const
{
   if (hotkeynum == 1) return Hotkey1Text;
   else if (hotkeynum == 2) return Hotkey2Text;
   else if (hotkeynum == 3) return Hotkey3Text;
   else if (hotkeynum == 4) return Hotkey4Text;
   else if (hotkeynum == 5) return Hotkey5Text;

   //assert(hotkeynum > 0 && hotkeynum < 6);
   return "";
}

void InfoWriterSettings::SetFilename(std::string filename)
{
   this->Filename = filename;
}

void InfoWriterSettings::SetFormat(std::string format)
{
   this->Format = format;
}

void InfoWriterSettings::SetHotkeyText(const int hotkeynum, std::string text)
{
   if (hotkeynum == 1) Hotkey1Text = text;
   else if (hotkeynum == 2) Hotkey2Text = text;
   else if (hotkeynum == 3) Hotkey3Text = text;
   else if (hotkeynum == 4) Hotkey4Text = text;
   else if (hotkeynum == 5) Hotkey5Text = text;
}
