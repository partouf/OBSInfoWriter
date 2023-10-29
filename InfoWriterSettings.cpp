#include "InfoWriterSettings.h"

std::string InfoWriterSettings::GetOutputFormat() const
{
	return OutputFormat;
}

bool InfoWriterSettings::GetShouldSyncNameAndPathWithVideo() const
{
	return ShouldSyncNameAndPathWithVideo;
}

std::string InfoWriterSettings::GetAutomaticOutputExtension() const
{
	return AutomaticOutputExtension;
}

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
	if (hotkeynum == 1)
		return Hotkey1Text;
	else if (hotkeynum == 2)
		return Hotkey2Text;
	else if (hotkeynum == 3)
		return Hotkey3Text;
	else if (hotkeynum == 4)
		return Hotkey4Text;
	else if (hotkeynum == 5)
		return Hotkey5Text;
	else if (hotkeynum == 6)
		return Hotkey6Text;
	else if (hotkeynum == 7)
		return Hotkey7Text;
	else if (hotkeynum == 8)
		return Hotkey8Text;
	else if (hotkeynum == 9)
		return Hotkey9Text;
	else if (hotkeynum == 10)
		return Hotkey10Text;
	else if (hotkeynum == 11)
		return Hotkey11Text;
	else if (hotkeynum == 12)
		return Hotkey12Text;
	else if (hotkeynum == 13)
		return Hotkey13Text;
	else if (hotkeynum == 14)
		return Hotkey14Text;

	//assert(hotkeynum > 0 && hotkeynum < 6);
	return "";
}

bool InfoWriterSettings::GetShouldLogSceneChanges() const
{
	return ShouldLogSceneChanges;
}

bool InfoWriterSettings::GetShouldLogStreaming() const
{
	return ShouldLogStreaming;
}

bool InfoWriterSettings::GetShouldLogAbsoluteTime() const
{
	return this->ShouldLogAbsoluteTime;
}

bool InfoWriterSettings::GetShouldLogHotkeySpecifics() const
{
	return this->ShouldLogHotkeySpecifics;
}

void InfoWriterSettings::SetOutputFormat(std::string formatname)
{
	OutputFormat = formatname;
}

void InfoWriterSettings::SetShouldSyncNameAndPathWithVideo(bool sync)
{
	ShouldSyncNameAndPathWithVideo = sync;
}

void InfoWriterSettings::SetAutomaticOutputExtension(std::string extension)
{
	AutomaticOutputExtension = extension;
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
	if (hotkeynum == 1)
		Hotkey1Text = text;
	else if (hotkeynum == 2)
		Hotkey2Text = text;
	else if (hotkeynum == 3)
		Hotkey3Text = text;
	else if (hotkeynum == 4)
		Hotkey4Text = text;
	else if (hotkeynum == 5)
		Hotkey5Text = text;
	else if (hotkeynum == 6)
		Hotkey6Text = text;
	else if (hotkeynum == 7)
		Hotkey7Text = text;
	else if (hotkeynum == 8)
		Hotkey8Text = text;
	else if (hotkeynum == 9)
		Hotkey9Text = text;
	else if (hotkeynum == 10)
		Hotkey10Text = text;
	else if (hotkeynum == 11)
		Hotkey11Text = text;
	else if (hotkeynum == 12)
		Hotkey12Text = text;
	else if (hotkeynum == 13)
		Hotkey13Text = text;
	else if (hotkeynum == 14)
		Hotkey14Text = text;
}

void InfoWriterSettings::SetShouldLogSceneChanges(bool logchanges)
{
	ShouldLogSceneChanges = logchanges;
}

void InfoWriterSettings::SetShouldLogStreaming(bool logchanges)
{
	ShouldLogStreaming = logchanges;
}

void InfoWriterSettings::SetShouldLogAbsoluteTime(bool log)
{
	this->ShouldLogAbsoluteTime = log;
}

void InfoWriterSettings::SetShouldLogHotkeySpecifics(bool log)
{
	this->ShouldLogHotkeySpecifics = log;
}
