#pragma once

#include <cstdint>
#include <string>

std::string FormatMillisToHMS(const std::string &format, int64_t totalmilliseconds);
bool HasUnsafeFormatSpecifiers(const std::string &format);
int CountFormatSpecifiers(const std::string &format);
