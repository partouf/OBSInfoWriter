
// note: the external symbols are defined in obs-module.h

#include <obs-module.h>
#include <Groundfloor/Materials/FileWriter.h>
#include <Groundfloor/Materials/Functions.h>
#include <Groundfloor/Atoms/Defines.h>
#include "InfoWriter.h"

const char *infowriter_idname = "infowriter";
const char *logfile_filter = "All formats (*.*)";
const char *setting_file = "file";
const char *setting_format = "format";

void obstudio_infowriter_hotkey(void *data, obs_hotkey_id id, obs_hotkey_t *hotkey, bool pressed)
{
   UNUSED_PARAMETER(id);
   UNUSED_PARAMETER(hotkey);

   if (pressed)
   {
      InfoWriter *Writer = static_cast<InfoWriter *>(data);

      // note: there's currently no way to determine start/stop of stream/recording, so the first time you press the hotkey it will start as well
      if (!Writer->HasStarted())
      {
         Writer->MarkStart();
      }

      Writer->WriteInfo();
   }
}

void obstudio_infowriter_startstream(void *data, calldata_t *params)
{
   UNUSED_PARAMETER(params);

   InfoWriter *Writer = static_cast<InfoWriter *>(data);
   Writer->MarkStart();
}

const char *obstudio_infowriter_get_name(void *type_data)
{
   UNUSED_PARAMETER(type_data);

   return infowriter_idname;
}

void *obstudio_infowriter_create(obs_data_t *settings, obs_source_t *source)
{
   InfoWriter *Writer = new InfoWriter();

   UNUSED_PARAMETER(settings);

   obs_hotkey_register_source(source, "InfoWriter", "Write timestamp to file", obstudio_infowriter_hotkey, Writer);

   return Writer;
}

obs_properties_t *obstudio_infowriter_properties(void *unused)
{
   UNUSED_PARAMETER(unused);

   obs_properties_t *props = obs_properties_create();

   obs_properties_add_path(props, setting_file, obs_module_text("Logfile"), OBS_PATH_FILE_SAVE, logfile_filter, NULL);
   obs_properties_add_text(props, setting_format, obs_module_text("Format"), OBS_TEXT_DEFAULT);

   return props;
}

void obstudio_infowriter_get_defaults(obs_data_t *settings)
{
   obs_data_set_default_string(settings, setting_file, "/tmp/log.txt");
   obs_data_set_default_string(settings, setting_format, "%d:%02d:%02d");
}

void obstudio_infowriter_update(void *data, obs_data_t *settings)
{
   InfoWriter *Writer = static_cast<InfoWriter *>(data);

   const char *file = obs_data_get_string(settings, setting_file);
   const char *format = obs_data_get_string(settings, setting_format);

   Writer->GetSettings()->SetFilename(file);
   Writer->GetSettings()->SetFormat(format);
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
      Writer->MarkStop();
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
