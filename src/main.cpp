#include "daisy_seed.h"
#include "daisysp.h"
#include "app/App.h"

using namespace daisy;
using namespace daisysp;

// Global hardware + app
DaisySeed hw;
App       app;

// Forward audio callback to App
static void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                          AudioHandle::InterleavingOutputBuffer out,
                          size_t                                size)
{
    app.AudioCallback(in, out, size);
}

int main(void)
{
    hw.Configure();
    hw.Init();

    // Initialize the high-level application
    app.Init(hw);

    hw.StartAudio(AudioCallback);

    // Main loop handles non-audio tasks
    while(1)
    {
        app.Loop();
    }
}