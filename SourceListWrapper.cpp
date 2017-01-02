#include "SourceListWrapper.h"
#include <util/darray.h>

SourceListWrapper::SourceListWrapper()
{
   da_init(list.sources);

   obs_frontend_get_scenes(&list);
}

SourceListWrapper::~SourceListWrapper()
{
   obs_frontend_source_list_free(&list);
}

int SourceListWrapper::getSourceIndex(const obs_source_t *ASource) const
{
   for (int i = 0; i < list.sources.num; ++i)
   {
      if (list.sources.array[i] == ASource)
      {
         return i;
      }
   }

   return -1;
}

size_t SourceListWrapper::size() const
{
   return list.sources.num;
}

std::string GuessSceneName(const obs_source_t * scene)
{
   std::string scenename = "";

   auto allscenenames = obs_frontend_get_scene_names();

   SourceListWrapper SourceList;
   auto sourceidx = SourceList.getSourceIndex(scene);
   if (sourceidx == -1)
      return scenename;

   // looping here, because array might end earlier and index out of bounds can cause mayhem
   auto sceneidx = 0;
   while (allscenenames[sceneidx] != nullptr)
   {
      if (sourceidx == sceneidx)
      {
         scenename = allscenenames[sceneidx];
         break;
      }

      sceneidx++;
   }

   return scenename;
}
