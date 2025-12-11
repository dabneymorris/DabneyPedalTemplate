#pragma once

// Common types and enums shared across the pedal framework.

namespace types
{
    // Bypass state of the pedal
    enum class BypassState
    {
        Bypassed,
        Active,
    };

    // Simple UI pages (we'll expand this later)
    enum class UiPage
    {
        Home,
        PresetSelect,
        Edit,
    };
} // namespace types