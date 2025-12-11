#pragma once

// Stub for OLED rendering.
// Later this will wrap a Daisy display driver and draw text/graphics.
class OledView
{
  public:
    OledView() {}
    ~OledView() {}

    void Init()
    {
        // TODO: init display hardware here (I2C/SPI, etc.)
    }

    void Clear()
    {
        // TODO: clear screen
    }

    void DrawStartupScreen()
    {
        // TODO: draw "Dabney Pedal Template" or similar
    }

    void Present()
    {
        // TODO: send buffer to display
    }
};