
// Stuff to test without having to plug in

#include "InfoWriter.h"
#include <climits>

int main()
{
	InfoWriter Writer;

	auto Settings = Writer.GetSettings();
	Settings->SetFilename("./log %Y-%m-%d %H%M%S.txt");
	Settings->SetOutputFormat("edl");
	Settings->SetFormat("\\t%d:%02d:%02d");
	Settings->SetHotkeyText(1, "ONE");
	Settings->SetHotkeyText(2, "TWO");
	Settings->SetShouldLogHotkeySpecifics(true);
	Settings->SetShouldLogStreaming(true);

	Writer.MarkStart(imtStream);

	GFMillisleep(1000);
	Writer.WriteInfo(1);

	GFMillisleep(3000);

	Writer.WriteInfo(1);

	GFMillisleep(1000);

	Writer.MarkStop(imtStream);

	GFMillisleep(1000);

	Writer.WriteInfo(2);

	Writer.SetSceneIsChanging(Writer.IsChangingScene());
	Writer.IsRecording();
	Writer.IsStreaming();

	return 0;
}
