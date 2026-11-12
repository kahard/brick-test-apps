#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "AudioServices.h"
#include "TextLines.h"
#include "brick/core/audio/PcmSource.h"
#include "brick/core/time/Timer.h"

namespace audio_test
{

/// A C major scale (C5..C6), one ToneSource per note played in turn.
class ScaleSource final : public brick::core::audio::IPcmSource
{
public:
    static constexpr std::size_t   kNotes      = 8;
    static constexpr std::uint32_t kNoteMs     = 220;
    static constexpr std::uint32_t kSampleRate = 22050;

    ScaleSource();
    brick::interfaces::audio::PcmFormat format() const override { return { kSampleRate, 1 }; }
    Status                              read(std::int16_t* frames, std::size_t capacity, std::size_t& count) override;
    Status                              rewind() override;

private:
    std::array<brick::core::audio::ToneSource, kNotes> notes_;
    std::size_t                                        note_ = 0;
};

/// Plays a tone, a scale and a WAV chime (from flash) on the board's speaker,
/// chosen by touching a line; two more lines change the volume. The output
/// switches off (amplifier too) a second after a sound ends. The log reports
/// each sound with the rate the output took its frames at.
class AudioTest final
{
public:
    AudioTest(const Services& services, const Settings& settings);
    Status initialize();
    void   update();

private:
    enum class Sound : std::uint8_t
    {
        none,
        tone,
        scale,
        chime,
    };

    void play_(Sound sound);
    void finish_();
    void handle_touch_();
    void show_();

    Services                                              services_;
    Settings                                              settings_;
    network_test::TextLines                               lines_;
    brick::core::audio::PcmPlayer                         player_;
    brick::core::audio::ToneSource                        tone_;
    ScaleSource                                           scale_;
    brick::core::audio::MemoryBytes                       chime_bytes_;
    brick::core::audio::WavSource                         chime_;
    brick::core::time::Timer                              idle_timer_;
    brick::core::time::Timer                              screen_timer_;
    Sound                                                 playing_        = Sound::none;
    Status                                                last_status_    = Status::ok;
    std::uint32_t                                         started_ms_     = 0;
    std::uint64_t                                         started_frames_ = 0;
    std::uint32_t                                         last_rate_hz_   = 0;
    std::array<brick::interfaces::display::TouchPoint, 1> touch_points_{};
    bool                                                  was_touched_ = false;
};

}  // namespace audio_test
