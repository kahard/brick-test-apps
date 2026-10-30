#include "AssetPlayback.h"

#include "generated_assets.h"

namespace cyd_asset_stream_smoke
{
namespace
{
    const char* storage_name(std::uint8_t storage)
    {
        switch (storage)
        {
            case 0:
                return "flash";
            case 1:
                return "psram";
            case 2:
                return "sd";
            default:
                return "unknown";
        }
    }
}  // namespace

AssetPlayback::AssetPlayback(AssetSources& sources, FrameBuffers& buffers, FramebufferPresenter& presenter,
                             brick::interfaces::display::IDisplayDevice& display,
                             brick::interfaces::time::ITimeProvider& time, brick::interfaces::logging::ILogger& logger)
    : sources_(sources), buffers_(buffers), presenter_(presenter), streamer_(display), time_(time),
      benchmark_(time, logger)
{
}

bool AssetPlayback::initialize()
{
    return sources_.select(0U) != nullptr;
}

bool AssetPlayback::advance_mode()
{
    mode_.advance();
    if (sources_.select(mode_.storage()) != nullptr)
    {
        benchmark_.reset();
        return true;
    }
    fallback_to_flash();
    return false;
}

bool AssetPlayback::present_next()
{
    const bool                 second = (frame_ & 1U) != 0U;
    const generated_assets::Id id =
        mode_.backgrounds() ? (second ? generated_assets::Id::blue_background : generated_assets::Id::red_background) :
                              (second ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears);
    const brick::interfaces::display::AssetDescriptor* asset  = generated_assets::get(id);
    brick::interfaces::display::IAssetSource*          source = sources_.select(mode_.storage());
    if (asset == nullptr || source == nullptr)
        return false;

    const std::uint64_t load_started = time_.micros();
    const bool          loaded = streamer_.stream_to_buffer(*asset, *source, buffers_.back_buffer(), buffers_.scratch(),
                                                            buffers_.scratch_bytes());
    const std::uint64_t load_us         = time_.micros() - load_started;
    const std::uint64_t present_started = time_.micros();
    const bool          presented       = loaded && presenter_.present();
    const std::uint64_t present_us      = time_.micros() - present_started;

    if (!presented && mode_.storage() == 2U)
    {
        fallback_to_flash();
        return present_next();
    }
    if (!presented)
        return false;

    benchmark_.frame(storage_name(), load_us, present_us);
    ++frame_;
    return true;
}

const char* AssetPlayback::storage_name() const
{
    return cyd_asset_stream_smoke::storage_name(mode_.storage());
}

bool AssetPlayback::backgrounds() const
{
    return mode_.backgrounds();
}

void AssetPlayback::fallback_to_flash()
{
    sources_.fallback_to_flash();
    mode_.fallback_to_flash();
    benchmark_.reset();
}
}  // namespace cyd_asset_stream_smoke
