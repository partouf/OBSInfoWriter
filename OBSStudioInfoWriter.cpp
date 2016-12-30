
// note: the external symbols are defined in obs-module.h

#include <obs-module.h>
#include <obs-frontend-api/obs-frontend-api.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Atoms/Defines.h>
#include "InfoWriter.h"

const char *infowriter_idname = "infowriter";
const char *logfile_filter = "All formats (*.*)";
const char *setting_file = "file";
const char *setting_format = "format";
const char *setting_hotkey1text = "hotkey1text";
const char *setting_hotkey2text = "hotkey2text";
const char *setting_hotkey3text = "hotkey3text";
const char *setting_hotkey4text = "hotkey4text";
const char *setting_hotkey5text = "hotkey5text";

void obstudio_infowriter_write_hotkey1(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      Writer->WriteInfo(1);
   }
}

void obstudio_infowriter_write_hotkey2(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      Writer->WriteInfo(2);
   }
}

void obstudio_infowriter_write_hotkey3(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      Writer->WriteInfo(3);
   }
}

void obstudio_infowriter_write_hotkey4(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      Writer->WriteInfo(4);
   }
}

void obstudio_infowriter_write_hotkey5(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      Writer->WriteInfo(5);
   }
}

const char *obstudio_infowriter_get_name(void *type_data)
{
   UNUSED_PARAMETER(type_data);

   return infowriter_idname;
}

void obsstudio_infowriter_frontend_event_callback(enum obs_frontend_event event, void *private_data)
{
   InfoWriter *Writer = static_cast<InfoWriter *>(private_data);

   if (event == OBS_FRONTEND_EVENT_STREAMING_STARTED)
   {
      Writer->MarkStart(imtStream);
   }
   else if (event == OBS_FRONTEND_EVENT_RECORDING_STARTED)
   {
      Writer->MarkStart(imtRecording);
   }
   else if (event == OBS_FRONTEND_EVENT_STREAMING_STOPPED)
   {
      Writer->MarkStop(imtStream);
   }
   else if (event == OBS_FRONTEND_EVENT_RECORDING_STOPPED)
   {
      Writer->MarkStop(imtRecording);
   }
   else if (event == OBS_FRONTEND_EVENT_SCENE_CHANGED)
   {
      Writer->WriteInfo("Scene changed");
   }
}

void *obstudio_infowriter_create(obs_data_t *settings, obs_source_t *source)
{
   InfoWriter *Writer = new InfoWriter();

   UNUSED_PARAMETER(settings);

   obs_hotkey_register_source(source, "InfoWriter", "Hotkey 1", obstudio_infowriter_write_hotkey1, Writer);
   obs_hotkey_register_source(source, "InfoWriter.Hotkey2", "Hotkey 2", obstudio_infowriter_write_hotkey2, Writer);
   obs_hotkey_register_source(source, "InfoWriter.Hotkey3", "Hotkey 3", obstudio_infowriter_write_hotkey3, Writer);
   obs_hotkey_register_source(source, "InfoWriter.Hotkey4", "Hotkey 4", obstudio_infowriter_write_hotkey4, Writer);
   obs_hotkey_register_source(source, "InfoWriter.Hotkey5", "Hotkey 5", obstudio_infowriter_write_hotkey5, Writer);

   obs_frontend_add_event_callback(obsstudio_infowriter_frontend_event_callback, Writer);

   return Writer;
}

obs_properties_t *obstudio_infowriter_properties(void *unused)
{
   UNUSED_PARAMETER(unused);

   obs_properties_t *props = obs_properties_create();

   obs_properties_add_path(props, setting_file, obs_module_text("Logfile"), OBS_PATH_FILE_SAVE, logfile_filter, NULL);
   obs_properties_add_text(props, setting_format, obs_module_text("Format"), OBS_TEXT_DEFAULT);

   obs_properties_add_text(props, setting_hotkey1text, obs_module_text("Hotkey 1 text"), OBS_TEXT_DEFAULT);
   obs_properties_add_text(props, setting_hotkey2text, obs_module_text("Hotkey 2 text"), OBS_TEXT_DEFAULT);
   obs_properties_add_text(props, setting_hotkey3text, obs_module_text("Hotkey 3 text"), OBS_TEXT_DEFAULT);
   obs_properties_add_text(props, setting_hotkey4text, obs_module_text("Hotkey 4 text"), OBS_TEXT_DEFAULT);
   obs_properties_add_text(props, setting_hotkey5text, obs_module_text("Hotkey 5 text"), OBS_TEXT_DEFAULT);

   return props;
}

void obstudio_infowriter_get_defaults(obs_data_t *settings)
{
   obs_data_set_default_string(settings, setting_file, "/tmp/log.txt");
   obs_data_set_default_string(settings, setting_format, "%d:%02d:%02d");

   obs_data_set_default_string(settings, setting_hotkey1text, "");
   obs_data_set_default_string(settings, setting_hotkey2text, "Hotkey 2 was pressed");
   obs_data_set_default_string(settings, setting_hotkey3text, "Hotkey 3 was pressed");
   obs_data_set_default_string(settings, setting_hotkey4text, "Hotkey 4 was pressed");
   obs_data_set_default_string(settings, setting_hotkey5text, "Hotkey 5 was pressed");
}

void obstudio_infowriter_update(void *data, obs_data_t *settings)
{
   InfoWriter *Writer = static_cast<InfoWriter *>(data);

   const char *file = obs_data_get_string(settings, setting_file);
   const char *format = obs_data_get_string(settings, setting_format);

   auto WriterSettings = Writer->GetSettings();

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
   if (Writer != nullptr)
   {
      if (Writer->HasStarted())
      {
         Writer->MarkStop(imtUnknown);
      }

      obs_frontend_remove_event_callback(obsstudio_infowriter_frontend_event_callback, Writer);

      delete Writer;
   }
}

struct obs_source_info obstudio_infowriter_source;

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("infowriter", "en-US")

void obstudio_infowriter_setup()
{
   obstudio_infowriter_source.id = infowriter_idname;
   obstudio_infowriter_source.type = OBS_SOURCE_TYPE_INPUT;
   obstudio_infowriter_source.get_name = obstudio_infowriter_get_name;
   obstudio_infowriter_source.create = obstudio_infowriter_create;
   obstudio_infowriter_source.destroy = obstudio_infowriter_destroy;
   obstudio_infowriter_source.get_width = obstudio_infowriter_get_width;
   obstudio_infowriter_source.get_height = obstudio_infowriter_get_height;
   obstudio_infowriter_source.get_properties = obstudio_infowriter_properties;
   obstudio_infowriter_source.update = obstudio_infowriter_update;
   obstudio_infowriter_source.load = obstudio_infowriter_update;
   obstudio_infowriter_source.get_defaults = obstudio_infowriter_get_defaults;

   obs_register_source(&obstudio_infowriter_source);
}

bool obs_module_load(void)
{
   obstudio_infowriter_setup();

   return true;
}

void obs_module_unload(void)
{
}
