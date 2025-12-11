#pragma once

#include <cstdint>

// Simple analog potentiometer abstraction.
// Later we'll wire this to an ADC channel and add smoothing.
class Potentiometer
{
  public:
    Potentiometer() {}
    ~Potentiometer() {}

    void Init()
    {
        // TODO: configure ADC channel from HardwareConfig
    }

    void Update()
    {
        // TODO: read ADC, low-pass filter, update value_
    }

    // Normalized value [0.0, 1.0]
    float GetValue() const { return value_; }

  private:
    float value_{0.0f};
};