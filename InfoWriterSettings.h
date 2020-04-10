#pragma once

#include <string>

class InfoWriterSettings
{
protected:
   std::string OutputFormat;
   std::string Filename;
   std::string Format;

   std::string Hotkey1Text;
   std::string Hotkey2Text;
   std::string Hotkey3Text;
   std::string Hotkey4Text;
   std::string Hotkey5Text;
   std::string Hotkey6Text;
   std::string Hotkey7Text;
   std::string Hotkey8Text;
   std::string Hotkey9Text;
   std::string Hotkey10Text;
   std::string Hotkey11Text;
   std::string Hotkey12Text;
   std::string Hotkey13Text;
   std::string Hotkey14Text;

   bool ShouldLogSceneChanges;
   bool ShouldLogStreaming;

public:
   std::string GetOutputFormat() const;
   std::string GetFilename() const;
   std::string GetFormat() const;
   std::string GetHotkeyText(const int hotkeynum) const;

   bool GetShouldLogSceneChanges() const;
   bool GetShouldLogStreaming() const;

   void SetOutputFormat(std::string formatname);
   void SetFilename(std::string filename);
   void SetFormat(std::string format);
   void SetHotkeyText(const int hotkeynum, std::string text);
   void SetShouldLogSceneChanges(bool logchanges);
   void SetShouldLogStreaming(bool logchanges);
};
