#pragma once

#include <obs.h>
#include <obs-frontend-api/obs-frontend-api.h>
#include <string>

class SourceListWrapper
{
protected:
   obs_frontend_source_list list;
public:
   SourceListWrapper();
   ~SourceListWrapper();

   int getSourceIndex(const obs_source_t *ASource) const;

   size_t size() const;
};

std::string GuessSceneName(const obs_source_t *scene);
