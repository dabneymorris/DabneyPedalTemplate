#pragma once

#include <cstdint>

// Simple encoder abstraction with a push button.
// For now, this is a stub. We'll wire it to actual GPIO later.
class Encoder
{
  public:
    Encoder() {}
    ~Encoder() {}

    void Init()
    {
        // TODO: later: take pins from HardwareConfig and init Daisy's encoder
    }

    // Call regularly from UiController::Update()
    void Update()
    {
        // TODO: read hardware and update internal state
    }

    // Returns encoder delta since last Update (e.g. -1, 0, +1)
    int GetDelta() const
    {
        return 0; // stub
    }

    // Button states (we'll implement later)
    bool Pressed() const { return false; }
    bool Released() const { return false; }
    bool Held() const { return false; }
};