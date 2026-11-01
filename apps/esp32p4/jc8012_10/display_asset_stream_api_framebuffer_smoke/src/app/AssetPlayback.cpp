#include "AssetPlayback.h"

#include "AssetSources.h"
#include "FramebufferPresenter.h"
#include "generated_assets.h"

namespace asset_stream_smoke
{
AssetPlayback::AssetPlayback(AssetSources& sources, FramebufferPresenter& presenter)
    : sources_(sources), presenter_(presenter)
{
}

Status AssetPlayback::initialize()
{
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(generated_assets::Id::joy_tears);
    if (asset == nullptr)
        return Status::not_found;
    return presenter_.initialize(*asset, sources_.flash());
}

Status AssetPlayback::advance_mode()
{
    mode_.advance();
    storage_ = mode_.storage();
    if (sources_.select(storage_) != nullptr)
        return Status::ok;
    fallback_to_flash();
    return Status::error;
}

Status AssetPlayback::present_next()
{
    const bool                 second = (frame_ & 1U) != 0U;
    const generated_assets::Id id =
        backgrounds() ? (second ? generated_assets::Id::blue_background : generated_assets::Id::red_background) :
                        (second ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears);
    const brick::interfaces::display::AssetDescriptor* asset  = generated_assets::get(id);
    brick::interfaces::display::IAssetSource*          source = sources_.select(storage_);

    if (asset == nullptr)
        return Status::not_found;
    Status status = source != nullptr ? presenter_.present(*asset, *source) : Status::not_ready;
    // The SD card can disappear; fall back to the flash copy.
    if (!is_ok(status) && storage_ == 2U)
    {
        fallback_to_flash();
        source = sources_.select(storage_);
        status = source != nullptr ? presenter_.present(*asset, *source) : Status::not_ready;
    }
    if (is_ok(status))
        ++frame_;
    return status;
}

std::uint8_t AssetPlayback::storage() const
{
    return storage_;
}

bool AssetPlayback::backgrounds() const
{
    return mode_.backgrounds();
}

void AssetPlayback::fallback_to_flash()
{
    sources_.fallback_to_flash();
    storage_ = 0U;
    mode_.fallback_to_flash();
}
}  // namespace asset_stream_smoke
