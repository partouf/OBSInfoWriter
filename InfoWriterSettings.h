#pragma once

#include <string>

class InfoWriterSettings
{
protected:
   std::string Filename;
   std::string Format;

   std::string Hotkey1Text;
   std::string Hotkey2Text;
   std::string Hotkey3Text;
   std::string Hotkey4Text;
   std::string Hotkey5Text;
public:
   std::string GetFilename() const;
   std::string GetFormat() const;
   std::string GetHotkeyText(const int hotkeynum) const;

   void SetFilename(std::string filename);
   void SetFormat(std::string format);
   void SetHotkeyText(const int hotkeynum, std::string text);
};
