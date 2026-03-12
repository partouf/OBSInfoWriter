#pragma once

#include <cstdarg>

#define LOG_ERROR 100
#define LOG_WARNING 200

#ifdef __cplusplus
extern "C" {
#endif

static inline void blog([[maybe_unused]] int log_level, [[maybe_unused]] const char *format, ...) {}

#ifdef __cplusplus
}
#endif
