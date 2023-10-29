
// note: the external symbols are defined in obs-module.h

#include <obs-module.h>
#include <obs-frontend-api.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Atoms/Defines.h>
#include "InfoWriter.h"

const char *infowriter_idname = "Info Writer";
const char *logfile_filter = "All formats (*.*)";
const char *setting_automaticoutputextension = "automaticoutputextension";
const char *setting_file = "file";
const char *setting_format = "format";
const char *setting_syncnameandpathwithvideo = "syncnameandpathwithvideo";
const char *setting_hotkey1text = "hotkey1text";
const char *setting_hotkey2text = "hotkey2text";
const char *setting_hotkey3text = "hotkey3text";
const char *setting_hotkey4text = "hotkey4text";
const char *setting_hotkey5text = "hotkey5text";
const char *setting_hotkey6text = "hotkey6text";
const char *setting_hotkey7text = "hotkey7text";
const char *setting_hotkey8text = "hotkey8text";
const char *setting_hotkey9text = "hotkey9text";
const char *setting_hotkey10text = "hotkey10text";
const char *setting_hotkey11text = "hotkey11text";
const char *setting_hotkey12text = "hotkey12text";
const char *setting_hotkey13text = "hotkey13text";
const char *setting_hotkey14text = "hotkey14text";
const char *setting_outputformat = "outputformat";
const char *setting_shouldlogscenechanges = "logscenechanges";
const char *setting_shouldlogstreaming = "logstreaming";
const char *setting_shouldlogabsolutetime = "logabsolutetime";
const char *setting_shouldloghotkeyspecifics = "loghotkeyspecifics";

bool obstudio_infowriter_syncnameandpathwithvideo_property_modified(
	obs_properties_t *props, obs_property_t *property, obs_data_t *settings)
{
	obs_property_t *prop_setting_file =
		obs_properties_get(props, setting_file);
	const bool previously_enabled = obs_property_enabled(prop_setting_file);
	const bool sync_selected =
		obs_data_get_bool(settings, setting_syncnameandpathwithvideo);

	obs_property_set_enabled(prop_setting_file, !sync_selected);

	obs_property_t *prop_automaticoutputextension =
		obs_properties_get(props, setting_automaticoutputextension);
	obs_property_set_visible(prop_automaticoutputextension, sync_selected);

	return (previously_enabled != obs_property_enabled(prop_setting_file));
}

void obstudio_infowriter_write_hotkey1(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(1);
	}
}

void obstudio_infowriter_write_hotkey2(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(2);
	}
}

void obstudio_infowriter_write_hotkey3(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(3);
	}
}

void obstudio_infowriter_write_hotkey4(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(4);
	}
}

void obstudio_infowriter_write_hotkey5(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(5);
	}
}

void obstudio_infowriter_write_hotkey6(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(6);
	}
}

void obstudio_infowriter_write_hotkey7(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(7);
	}
}
void obstudio_infowriter_write_hotkey8(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(8);
	}
}
void obstudio_infowriter_write_hotkey9(void *data, obs_hotkey_id id,
				       obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(9);
	}
}
void obstudio_infowriter_write_hotkey10(void *data, obs_hotkey_id id,
					obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(10);
	}
}
void obstudio_infowriter_write_hotkey11(void *data, obs_hotkey_id id,
					obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(11);
	}
}
void obstudio_infowriter_write_hotkey12(void *data, obs_hotkey_id id,
					obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(12);
	}
}
void obstudio_infowriter_write_hotkey13(void *data, obs_hotkey_id id,
					obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(13);
	}
}
void obstudio_infowriter_write_hotkey14(void *data, obs_hotkey_id id,
					obs_hotkey_t *hotkey, bool pressed)
{
	UNUSED_PARAMETER(id);
	UNUSED_PARAMETER(hotkey);

	if (pressed) {
		InfoWriter *Writer = static_cast<InfoWriter *>(data);

		Writer->WriteInfo(14);
	}
}

const char *obstudio_infowriter_get_name(void *type_data)
{
	UNUSED_PARAMETER(type_data);

	return infowriter_idname;
}

void LogSceneChange(InfoWriter *Writer, const std::string scenename)
{
	auto WriterSettings = Writer->GetSettings();

	if (WriterSettings->GetShouldLogSceneChanges()) {
		Writer->WriteSceneChange(scenename);
	}
}

void obsstudio_infowriter_frontend_event_callback(enum obs_frontend_event event,
						  void *private_data)
{
	InfoWriter *Writer = static_cast<InfoWriter *>(private_data);

	if (event == OBS_FRONTEND_EVENT_STREAMING_STARTED) {
		Writer->MarkStart(imtStream);
	} else if (event == OBS_FRONTEND_EVENT_RECORDING_STARTED) {
		Writer->MarkStart(imtRecording);
	} else if (event == OBS_FRONTEND_EVENT_STREAMING_STOPPED) {
		if (Writer->IsStreaming())
			Writer->MarkStop(imtStream);
	} else if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED) {
		Writer->MarkStop(imtRecording);
	} else if (event == OBS_FRONTEND_EVENT_RECORDING_PAUSED) {
		Writer->MarkPauseStart(imtRecordingPauseStart);
	} else if (event == OBS_FRONTEND_EVENT_RECORDING_UNPAUSED) {
		Writer->MarkPauseResume(imtRecordingPauseResume);
	} else if (event == OBS_FRONTEND_EVENT_SCENE_CHANGED) {
		auto scene = obs_frontend_get_current_scene();
		std::string scenename = obs_source_get_name(scene);
		obs_source_release(scene);

		LogSceneChange(Writer, scenename);
	}
}

void *obstudio_infowriter_create(obs_data_t *settings, obs_source_t *source)
{
	InfoWriter *Writer = new InfoWriter();

	UNUSED_PARAMETER(settings);

	obs_hotkey_register_source(source, "InfoWriter", "Hotkey 1",
				   obstudio_infowriter_write_hotkey1, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey2", "Hotkey 2",
				   obstudio_infowriter_write_hotkey2, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey3", "Hotkey 3",
				   obstudio_infowriter_write_hotkey3, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey4", "Hotkey 4",
				   obstudio_infowriter_write_hotkey4, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey5", "Hotkey 5",
				   obstudio_infowriter_write_hotkey5, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey6", "Hotkey 6",
				   obstudio_infowriter_write_hotkey6, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey7", "Hotkey 7",
				   obstudio_infowriter_write_hotkey7, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey8", "Hotkey 8",
				   obstudio_infowriter_write_hotkey8, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey9", "Hotkey 9",
				   obstudio_infowriter_write_hotkey9, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey10", "Hotkey 10",
				   obstudio_infowriter_write_hotkey10, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey11", "Hotkey 11",
				   obstudio_infowriter_write_hotkey11, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey12", "Hotkey 12",
				   obstudio_infowriter_write_hotkey12, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey13", "Hotkey 13",
				   obstudio_infowriter_write_hotkey13, Writer);
	obs_hotkey_register_source(source, "InfoWriter.Hotkey14", "Hotkey 14",
				   obstudio_infowriter_write_hotkey14, Writer);

	obs_frontend_add_event_callback(
		obsstudio_infowriter_frontend_event_callback, Writer);

	return Writer;
}

obs_properties_t *obstudio_infowriter_properties(void *unused)
{
	UNUSED_PARAMETER(unused);

	obs_properties_t *props = obs_properties_create();

	auto list = obs_properties_add_list(props, setting_outputformat,
					    obs_module_text("Output format"),
					    OBS_COMBO_TYPE_LIST,
					    OBS_COMBO_FORMAT_STRING);
	obs_property_list_add_string(list, "Default", "default");
	obs_property_list_add_string(list, "CSV", "csv");
	obs_property_list_add_string(list, "EDL", "edl");
	obs_property_list_add_string(list, "SRT", "srt");

	obs_properties_add_text(props, setting_format,
				obs_module_text("Format"), OBS_TEXT_DEFAULT);
	obs_property *prop_syncnameandpathwithvideo =
		obs_properties_add_bool(props, setting_syncnameandpathwithvideo,
					"Sync with video file name and path");
	obs_property_set_modified_callback(
		prop_syncnameandpathwithvideo,
		obstudio_infowriter_syncnameandpathwithvideo_property_modified);
	obs_properties_add_text(props, setting_automaticoutputextension,
				"Automatic file extension", OBS_TEXT_DEFAULT);
	obs_properties_add_path(props, setting_file, obs_module_text("Logfile"),
				OBS_PATH_FILE_SAVE, logfile_filter, NULL);

	obs_properties_add_text(props, setting_hotkey1text,
				obs_module_text("Hotkey 1 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey2text,
				obs_module_text("Hotkey 2 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey3text,
				obs_module_text("Hotkey 3 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey4text,
				obs_module_text("Hotkey 4 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey5text,
				obs_module_text("Hotkey 5 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey6text,
				obs_module_text("Hotkey 6 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey7text,
				obs_module_text("Hotkey 7 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey8text,
				obs_module_text("Hotkey 8 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey9text,
				obs_module_text("Hotkey 9 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey10text,
				obs_module_text("Hotkey 10 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey11text,
				obs_module_text("Hotkey 11 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey12text,
				obs_module_text("Hotkey 12 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey13text,
				obs_module_text("Hotkey 13 text"),
				OBS_TEXT_DEFAULT);
	obs_properties_add_text(props, setting_hotkey14text,
				obs_module_text("Hotkey 14 text"),
				OBS_TEXT_DEFAULT);

	obs_properties_add_bool(props, setting_shouldlogscenechanges,
				obs_module_text("Log Scene changes"));
	obs_properties_add_bool(
		props, setting_shouldlogstreaming,
		obs_module_text(
			"Log Streaming events (applies to default output)"));
	obs_properties_add_bool(
		props, setting_shouldlogabsolutetime,
		obs_module_text(
			"Log intermediate absolute times (applies to default output)"));
	obs_properties_add_bool(
		props, setting_shouldloghotkeyspecifics,
		obs_module_text(
			"Log hotkey specifics (applies to default output)"));

	return props;
}

void obstudio_infowriter_get_defaults(obs_data_t *settings)
{
	obs_data_set_default_string(settings, setting_outputformat, "default");
	obs_data_set_default_bool(settings, setting_syncnameandpathwithvideo,
				  false);
	obs_data_set_default_string(settings, setting_automaticoutputextension, "txt");
	obs_data_set_default_string(settings, setting_file, "/tmp/log.txt");
	obs_data_set_default_string(settings, setting_format, "%d:%02d:%02d");

	obs_data_set_default_string(settings, setting_hotkey1text, "");
	obs_data_set_default_string(settings, setting_hotkey2text,
				    "Hotkey 2 was pressed");
	obs_data_set_default_string(settings, setting_hotkey3text,
				    "Hotkey 3 was pressed");
	obs_data_set_default_string(settings, setting_hotkey4text,
				    "Hotkey 4 was pressed");
	obs_data_set_default_string(settings, setting_hotkey5text,
				    "Hotkey 5 was pressed");
	obs_data_set_default_string(settings, setting_hotkey6text,
				    "Hotkey 6 was pressed");
	obs_data_set_default_string(settings, setting_hotkey7text,
				    "Hotkey 7 was pressed");
	obs_data_set_default_string(settings, setting_hotkey8text,
				    "Hotkey 8 was pressed");
	obs_data_set_default_string(settings, setting_hotkey9text,
				    "Hotkey 9 was pressed");
	obs_data_set_default_string(settings, setting_hotkey10text,
				    "Hotkey 10 was pressed");
	obs_data_set_default_string(settings, setting_hotkey11text,
				    "Hotkey 11 was pressed");
	obs_data_set_default_string(settings, setting_hotkey12text,
				    "Hotkey 12 was pressed");
	obs_data_set_default_string(settings, setting_hotkey13text,
				    "Hotkey 13 was pressed");
	obs_data_set_default_string(settings, setting_hotkey14text,
				    "Hotkey 14 was pressed");

	obs_data_set_default_bool(settings, setting_shouldlogscenechanges,
				  true);
	obs_data_set_default_bool(settings, setting_shouldlogstreaming, false);
	obs_data_set_default_bool(settings, setting_shouldlogabsolutetime,
				  true);
	obs_data_set_default_bool(settings, setting_shouldloghotkeyspecifics,
				  true);
}

void obstudio_infowriter_update(void *data, obs_data_t *settings)
{
	InfoWriter *Writer = static_cast<InfoWriter *>(data);

	const char *outputformat =
		obs_data_get_string(settings, setting_outputformat);

	const char *automaticextension =
		obs_data_get_string(settings, setting_automaticoutputextension);
	const char *file = obs_data_get_string(settings, setting_file);
	const char *format = obs_data_get_string(settings, setting_format);

	auto WriterSettings = Writer->GetSettings();

	WriterSettings->SetOutputFormat(outputformat);
	WriterSettings->SetShouldSyncNameAndPathWithVideo(
		obs_data_get_bool(settings, setting_syncnameandpathwithvideo));
	WriterSettings->SetAutomaticOutputExtension(automaticextension);
	WriterSettings->SetFilename(file);
	WriterSettings->SetFormat(format);

	const char *hotkeytext;

	hotkeytext = obs_data_get_string(settings, setting_hotkey1text);
	WriterSettings->SetHotkeyText(1, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey2text);
	WriterSettings->SetHotkeyText(2, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey3text);
	WriterSettings->SetHotkeyText(3, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey4text);
	WriterSettings->SetHotkeyText(4, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey5text);
	WriterSettings->SetHotkeyText(5, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey6text);
	WriterSettings->SetHotkeyText(6, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey7text);
	WriterSettings->SetHotkeyText(7, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey8text);
	WriterSettings->SetHotkeyText(8, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey9text);
	WriterSettings->SetHotkeyText(9, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey10text);
	WriterSettings->SetHotkeyText(10, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey11text);
	WriterSettings->SetHotkeyText(11, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey12text);
	WriterSettings->SetHotkeyText(12, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey13text);
	WriterSettings->SetHotkeyText(13, hotkeytext);

	hotkeytext = obs_data_get_string(settings, setting_hotkey14text);
	WriterSettings->SetHotkeyText(14, hotkeytext);

	WriterSettings->SetShouldLogSceneChanges(
		obs_data_get_bool(settings, setting_shouldlogscenechanges));
	WriterSettings->SetShouldLogStreaming(
		obs_data_get_bool(settings, setting_shouldlogstreaming));
	WriterSettings->SetShouldLogAbsoluteTime(
		obs_data_get_bool(settings, setting_shouldlogabsolutetime));
	WriterSettings->SetShouldLogHotkeySpecifics(
		obs_data_get_bool(settings, setting_shouldloghotkeyspecifics));
}

uint32_t obstudio_infowriter_get_width(void *data)
{
	UNUSED_PARAMETER(data);

	return 0;
}

uint32_t obstudio_infowriter_get_height(void *data)
{
	UNUSED_PARAMETER(data);

	return 0;
}

void obstudio_infowriter_destroy(void *data)
{
	InfoWriter *Writer = static_cast<InfoWriter *>(data);
	if (Writer != nullptr) {
		if (Writer->HasStarted()) {
			Writer->MarkStop(imtUnknown);
		}

		obs_frontend_remove_event_callback(
			obsstudio_infowriter_frontend_event_callback, Writer);

		delete Writer;
	}
}

struct obs_source_info obstudio_infowriter_source;

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("OBSInfoWriter", "en-US")

void obstudio_infowriter_setup()
{
	obstudio_infowriter_source.id = infowriter_idname;
	obstudio_infowriter_source.type = OBS_SOURCE_TYPE_INPUT;
	obstudio_infowriter_source.get_name = obstudio_infowriter_get_name;
	obstudio_infowriter_source.create = obstudio_infowriter_create;
	obstudio_infowriter_source.destroy = obstudio_infowriter_destroy;
	obstudio_infowriter_source.get_width = obstudio_infowriter_get_width;
	obstudio_infowriter_source.get_height = obstudio_infowriter_get_height;
	obstudio_infowriter_source.get_properties =
		obstudio_infowriter_properties;
	obstudio_infowriter_source.update = obstudio_infowriter_update;
	obstudio_infowriter_source.load = obstudio_infowriter_update;
	obstudio_infowriter_source.get_defaults =
		obstudio_infowriter_get_defaults;

	obs_register_source(&obstudio_infowriter_source);
}

bool obs_module_load(void)
{
	obstudio_infowriter_setup();

	return true;
}

void obs_module_unload(void) {}
