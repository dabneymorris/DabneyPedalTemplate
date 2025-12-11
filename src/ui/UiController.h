#pragma once

#include "ui/Encoder.h"
#include "ui/Footswitch.h"
#include "ui/OledView.h"
#include "ui/Button.h"
#include "ui/ToggleSwitch.h"
#include "ui/Potentiometer.h"
#include "ui/Led.h"
#include "util/Types.h"

// UiController manages interaction:
// - Reads encoder / footswitch / buttons / toggles / pots
// - Chooses what to show on OLED
// For now it's a stub that does nothing visual.
class UiController
{
  public:
    UiController() {}
    ~UiController() {}

    void Init()
    {
        encoder_.Init();
        footswitch_.Init();
        button_.Init();
        toggle_.Init();
        pot_.Init();
        led_.Init();
        oled_.Init();

        current_page_ = types::UiPage::Home;
        // Later: draw an initial screen here.
    }

    // Called regularly from App::Loop()
    void Update()
    {
        encoder_.Update();
        footswitch_.Update();
        button_.Update();
        toggle_.Update();
        pot_.Update();
        // LED is driven by other parts of the app; no Update() needed.
        // oled_ rendering will go here later.
    }

  private:
    Encoder        encoder_;
    Footswitch     footswitch_;
    Button         button_;
    ToggleSwitch   toggle_;
    Potentiometer  pot_;
    Led            led_;
    OledView       oled_;
    types::UiPage  current_page_{types::UiPage::Home};
};