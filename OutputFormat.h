#pragma once

#include <string>

class IOutputFormat {
public:
   virtual void Start() = 0;
   virtual void Stop(const int64_t timestamp) = 0;

   virtual void HotkeyMarker(const int64_t timestamp, const std::string text) = 0;
   virtual void ScenechangeMarker(const int64_t timestamp, const std::string scenename) = 0;
   virtual void PausedMarker(const int64_t timestamp) = 0;
   virtual void ResumedMarker(const int64_t timestamp, const int64_t pauselength) = 0;

   virtual void TextMarker(const std::string text) = 0;
};
