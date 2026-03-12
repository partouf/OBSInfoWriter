#pragma once

#include <cstdint>
#include <string>

class IOutputFormat {
public:
	virtual ~IOutputFormat() = default;

	virtual void Start() = 0;
	virtual void Stop(const int64_t milliseconds) = 0;

	virtual void HotkeyMarker(const int64_t milliseconds,
				  const std::string text) = 0;
	virtual void ScenechangeMarker(const int64_t milliseconds,
				       const std::string scenename) = 0;
	virtual void PausedMarker(const int64_t milliseconds) = 0;
	virtual void ResumedMarker(const int64_t milliseconds,
				   const int64_t pauselength_ms) = 0;

	virtual void TextMarker(const std::string text) = 0;
};
