#pragma once

#include <array>
#include <cstdint>

#include "ApplicationConfig.h"
#include "AudioTest.h"

using Status = brick::Status;

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const audio_test::Services& services)
        : test_(services, { kBoardName, kLineHeight, line_pixels_.data(), line_pixels_.size() })
    {
    }

    Status initialize() { return test_.initialize(); }
    void   update() { test_.update(); }

private:
    std::array<std::uint16_t, kScreenWidth* static_cast<std::size_t>(kLineHeight)> line_pixels_{};
    audio_test::AudioTest                                                          test_;
};
