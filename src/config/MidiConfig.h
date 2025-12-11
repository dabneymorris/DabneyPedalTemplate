#pragma once

#include <cstdint>

namespace config
{
namespace midi
{
    // 0 = omni (listen to all channels)
    // 1-16 = specific MIDI channel
    static constexpr uint8_t kDefaultChannel = 1;

} // namespace midi
} // namespace config