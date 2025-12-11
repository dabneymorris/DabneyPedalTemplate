#pragma once

// Simple debounced momentary button abstraction.
// Later we'll wire this to a GPIO pin from HardwareConfig.
class Button
{
  public:
    Button() {}
    ~Button() {}

    void Init()
    {
        // TODO: initialize GPIO from HardwareConfig
    }

    void Update()
    {
        // TODO: read pin, debounce, update edge/hold state
    }

    // Edge: went from up to down on this Update()
    bool Pressed() const { return false; }

    // Edge: went from down to up on this Update()
    bool Released() const { return false; }

    // Level: currently pressed
    bool Held() const { return false; }
};