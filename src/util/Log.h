#pragma once

#include "config/BuildConfig.h"
#include <cstdarg>

// Simple logging helper.
// Right now it does nothing, but we can wire it to daisy::DebugLog or UART later.
namespace logutil
{
    // Print a formatted message (printf-style)
    void Logf(const char* fmt, ...);

} // namespace logutil

// Convenience macro
#if ENABLE_LOGGING
#define LOGF(...) ::logutil::Logf(__VA_ARGS__)
#else
#define LOGF(...) \
    do            \
    {             \
    } while(0)
#endif