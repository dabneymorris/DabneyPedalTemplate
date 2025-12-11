#pragma once

// Simple LED abstraction.
// Later we can support PWM for brightness.
class Led
{
  public:
    Led() {}
    ~Led() {}

    void Init()
    {
        // TODO: configure GPIO (or PWM) from HardwareConfig
    }

    void Set(bool on)
    {
        // TODO: drive GPIO here
        (void)on;
        state_ = on;
    }

    bool Get() const { return state_; }

  private:
    bool state_{false};
};