#pragma once

#include <cstdint>

// Simple debounced footswitch abstraction.
// We'll wire this to a GPIO pin later.
class Footswitch
{
  public:
    Footswitch() {}
    ~Footswitch() {}

    void Init()
    {
        // TODO: later: initialize GPIO from HardwareConfig
    }

    void Update()
    {
        // TODO: read pin, debounce, update edge/hold state
    }

    bool Pressed() const { return false; }   // edge: went down
    bool Released() const { return false; }  // edge: went up
    bool Held() const { return false; }      // level: currently down
};