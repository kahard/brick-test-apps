#include "AudioTest.h"

#include "chime_wav.h"

namespace audio_test
{

namespace
{
    constexpr char          kTag[]      = "audio_test";
    constexpr std::uint32_t kIdleOffMs  = 1000;  ///< silence before the output (amplifier) switches off
    constexpr std::uint32_t kScreenMs   = 250;
    constexpr std::uint8_t  kVolumeStep = 10;
    constexpr std::uint32_t kToneHz     = 440;
    constexpr std::uint32_t kToneMs     = 1500;
    constexpr std::uint32_t kToneRate   = 22050;

    constexpr std::uint16_t kWhite  = 0xFFFF;
    constexpr std::uint16_t kGray   = 0x8410;
    constexpr std::uint16_t kGreen  = 0x07E0;
    constexpr std::uint16_t kYellow = 0xFFE0;
    constexpr std::uint16_t kCyan   = 0x07FF;
    constexpr std::uint16_t kRed    = 0xF800;

    // Screen lines; the ones from kFirstAction are touched to act.
    constexpr std::size_t kTitleLine   = 0;
    constexpr std::size_t kOutputLine  = 1;
    constexpr std::size_t kStateLine   = 2;
    constexpr std::size_t kRateLine    = 3;
    constexpr std::size_t kToneLine    = 4;
    constexpr std::size_t kScaleLine   = 5;
    constexpr std::size_t kChimeLine   = 6;
    constexpr std::size_t kQuieterLine = 7;
    constexpr std::size_t kLouderLine  = 8;
    constexpr std::size_t kStopLine    = 9;  // the tenth line: fits the CYD's 240 pixels at 22 per line

    constexpr std::uint32_t kScaleHz[ScaleSource::kNotes] = { 523, 587, 659, 698, 784, 880, 988, 1047 };

    constexpr const char* kSoundNames[] = { "-", "tone 440 Hz", "C major scale", "chime (WAV)" };

    const char* name_of(int sound)
    {
        return kSoundNames[sound];
    }
}  // namespace

// ---- ScaleSource --------------------------------------------------------------

ScaleSource::ScaleSource()
    : notes_{ { { kScaleHz[0], kNoteMs, kSampleRate },
                { kScaleHz[1], kNoteMs, kSampleRate },
                { kScaleHz[2], kNoteMs, kSampleRate },
                { kScaleHz[3], kNoteMs, kSampleRate },
                { kScaleHz[4], kNoteMs, kSampleRate },
                { kScaleHz[5], kNoteMs, kSampleRate },
                { kScaleHz[6], kNoteMs, kSampleRate },
                { kScaleHz[7], kNoteMs, kSampleRate } } }
{
}

Status ScaleSource::read(std::int16_t* frames, std::size_t capacity, std::size_t& count)
{
    count = 0;
    while (note_ < kNotes)
    {
        const Status status = notes_[note_].read(frames, capacity, count);
        if (is_ok(status) && count > 0)
            return Status::ok;
        ++note_;
    }
    return Status::no_data;
}

Status ScaleSource::rewind()
{
    for (auto& note : notes_)
        (void)note.rewind();
    note_ = 0;
    return Status::ok;
}

// ---- AudioTest -----------------------------------------------------------------

AudioTest::AudioTest(const Services& services, const Settings& settings)
    : services_(services), settings_(settings),
      lines_(services.display, settings.line_height, settings.line_pixels, settings.line_capacity),
      player_(services.audio), tone_(kToneHz, kToneMs, kToneRate), chime_bytes_(kChimeWav, kChimeWavSize),
      chime_(chime_bytes_), idle_timer_(services.time), screen_timer_(services.time)
{
}

Status AudioTest::initialize()
{
    if (const Status status = lines_.clear(); !is_ok(status))
        return status;
    if (const Status status = chime_.open(); !is_ok(status))
        services_.logger.error(kTag, "The chime WAV does not open: %s", to_string(status));
    services_.logger.info(kTag, "Audio test on %s: touch a line to play; the scale plays first", settings_.board);
    play_(Sound::scale);
    screen_timer_.start(kScreenMs);
    show_();
    return lines_.take_failure();
}

void AudioTest::update()
{
    player_.update();
    if (playing_ != Sound::none && !player_.playing())
        finish_();
    if (playing_ == Sound::none && idle_timer_.expired())
    {
        idle_timer_.stop();
        services_.audio.stop();
        services_.logger.info(kTag, "Output off");
    }
    handle_touch_();
    if (screen_timer_.expired())
    {
        screen_timer_.restart();
        show_();
    }
}

void AudioTest::play_(Sound sound)
{
    brick::core::audio::IPcmSource* source = nullptr;
    switch (sound)
    {
        case Sound::tone:
            source = &tone_;
            break;
        case Sound::scale:
            source = &scale_;
            break;
        case Sound::chime:
            source = &chime_;
            break;
        case Sound::none:
            return;
    }
    (void)source->rewind();
    idle_timer_.stop();
    started_ms_     = services_.time.millis();
    started_frames_ = player_.frames_sent();
    last_status_    = player_.play(*source);
    if (!is_ok(last_status_))
    {
        services_.logger.error(kTag, "Playing the %s failed: %s", name_of(static_cast<int>(sound)),
                               to_string(last_status_));
        playing_ = Sound::none;
        return;
    }
    playing_ = sound;
    services_.logger.info(kTag, "Playing the %s (%lu Hz, volume %u%%)", name_of(static_cast<int>(sound)),
                          static_cast<unsigned long>(source->format().sample_rate),
                          static_cast<unsigned>(services_.audio.volume()));
}

void AudioTest::finish_()
{
    const std::uint32_t elapsed = services_.time.millis() - started_ms_;
    const std::uint64_t frames  = player_.frames_sent() - started_frames_;
    // The output takes frames as fast as it plays them (once its buffers are
    // full), so this is about its sample rate.
    last_rate_hz_ = elapsed == 0 ? 0 : static_cast<std::uint32_t>(frames * 1000 / elapsed);
    services_.logger.info(kTag, "Played the %s: %lu frames in %lu ms (about %lu Hz)",
                          name_of(static_cast<int>(playing_)), static_cast<unsigned long>(frames),
                          static_cast<unsigned long>(elapsed), static_cast<unsigned long>(last_rate_hz_));
    playing_ = Sound::none;
    idle_timer_.start(kIdleOffMs);
}

void AudioTest::handle_touch_()
{
    if (services_.touch == nullptr)
        return;
    std::size_t  count  = 0;
    const Status status = services_.touch->read(touch_points_.data(), touch_points_.size(), count);
    // no_data: no new sample since the last read (a GT911 samples about every
    // 10 ms), so the finger is where it was.
    if (status == Status::no_data)
        return;
    const bool touched =
        is_ok(status) && count > 0 && touch_points_[0].state != brick::interfaces::display::TouchState::released;
    const bool pressed = touched && !was_touched_;
    was_touched_       = touched;
    if (!pressed || settings_.line_height <= 0)
        return;
    const auto line = static_cast<std::size_t>(touch_points_[0].y / settings_.line_height);
    switch (line)
    {
        case kToneLine:
            play_(Sound::tone);
            break;
        case kScaleLine:
            play_(Sound::scale);
            break;
        case kChimeLine:
            play_(Sound::chime);
            break;
        case kQuieterLine:
        case kLouderLine:
        {
            const int step   = line == kLouderLine ? kVolumeStep : -kVolumeStep;
            int       volume = services_.audio.volume() + step;
            volume           = volume < 0 ? 0 : (volume > 100 ? 100 : volume);
            (void)services_.audio.set_volume(static_cast<std::uint8_t>(volume));
            services_.logger.info(kTag, "Volume %d%%", volume);
            break;
        }
        case kStopLine:
            player_.stop();
            break;
        default:
            break;
    }
}

void AudioTest::show_()
{
    lines_.show(kTitleLine, kWhite, "BRICK AUDIO TEST");
    const brick::interfaces::audio::PcmFormat format = services_.audio.format();
    if (format.sample_rate == 0)
        lines_.show(kOutputLine, kGray, "OUT  off  volume %u%%", static_cast<unsigned>(services_.audio.volume()));
    else
        lines_.show(kOutputLine, kGreen, "OUT  %lu Hz %s  volume %u%%", static_cast<unsigned long>(format.sample_rate),
                    format.channels == 2 ? "stereo" : "mono", static_cast<unsigned>(services_.audio.volume()));
    if (!is_ok(last_status_))
        lines_.show(kStateLine, kRed, "FAILED  %s", to_string(last_status_));
    else if (playing_ != Sound::none)
        lines_.show(kStateLine, kYellow, "PLAYING  %s", name_of(static_cast<int>(playing_)));
    else
        lines_.show(kStateLine, kGray, "IDLE");
    if (last_rate_hz_ != 0)
        lines_.show(kRateLine, kGray, "last: about %lu Hz", static_cast<unsigned long>(last_rate_hz_));
    lines_.show(kToneLine, kCyan, "> TONE 440 Hz");
    lines_.show(kScaleLine, kCyan, "> SCALE C major");
    lines_.show(kChimeLine, kCyan, "> CHIME (WAV from flash)");
    lines_.show(kQuieterLine, kCyan, "> VOLUME -");
    lines_.show(kLouderLine, kCyan, "> VOLUME +");
    lines_.show(kStopLine, kCyan, "> STOP");
    (void)lines_.take_failure();
}

}  // namespace audio_test
