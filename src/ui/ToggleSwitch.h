#pragma once

#include <cstdint>

// Simple 2-position toggle switch abstraction (e.g. up/down).
// We can extend this later for 3-position if needed.
class ToggleSwitch
{
  public:
    enum class Position : uint8_t
    {
        Down = 0,
        Up   = 1,
    };

    ToggleSwitch() {}
    ~ToggleSwitch() {}

    void Init()
    {
        // TODO: initialize GPIO(s) from HardwareConfig
    }

    void Update()
    {
        // TODO: read pin(s) and set position_
    }

    Position GetPosition() const { return position_; }

  private:
    Position position_{Position::Down};
};