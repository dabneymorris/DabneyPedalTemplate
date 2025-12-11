#pragma once

#include <cstddef>

class DspEngine
{
  public:
    DspEngine() {}
    ~DspEngine() {}

    // Call once at startup
    void Init(float sample_rate)
    {
        sample_rate_ = sample_rate;
    }

    // Process interleaved stereo audio in-place:
    // size = number of float samples in the interleaved buffer (L,R,L,R,...)
    void ProcessInterleaved(const float* in, float* out, std::size_t size)
    {
        // For now: pure passthrough
        for(std::size_t i = 0; i < size; i += 2)
        {
            float l = in[i];
            float r = in[i + 1];

            out[i]     = l;
            out[i + 1] = r;
        }
    }

  private:
    float sample_rate_{48000.0f};
};